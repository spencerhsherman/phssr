<?php
/**
 * Extension of QuizQuestionResponse
 */
class PhssrMultichoiceResponse extends QuizQuestionResponse {

  /**
   * ID of the answers.
   */
  protected $user_answer_ids;
  protected $choice_order;

  /**
   * Constructor.
   *
   * @param type $result_id
   * @param stdClass $question_node
   * @param type $values
   *   Form state values.
   */
  public function __construct($result_id, stdClass $question_node, $values = NULL) {
    parent::__construct($result_id, $question_node, $values['user_answer']);
    $this->user_answer_ids = array();
    // tries is the tries part of the post data
    if (isset($values['user_answer'])) {
      if (!is_array($values['user_answer'])) {
        // Account for single-select
        $values['user_answer'] = array($values['user_answer']);
      }
      if (isset($values['choice_order'])) {
        $this->choice_order = $values['choice_order'];
      }
      unset($values['choice_order']);
      if (isset($values['user_answer']) && is_array($values['user_answer'])) {
        foreach ($values['user_answer'] as $answer_id) {
          $this->user_answer_ids[] = $answer_id;
          $this->answer = $this->user_answer_ids; //@todo: Stop using user_answer_ids and only use answer instead...
        }
      }
      elseif (isset($values['user_answer'])) {
        $this->user_answer_ids[] = $values['user_answer'];
      }
    }
    // We load the answer from the database
    else if (isset($this->result_answer_id)) {
      $res = db_query('SELECT answer_id FROM {quiz_multichoice_user_answers} ua
        LEFT OUTER JOIN {quiz_multichoice_user_answer_multi} uam ON (uam.user_answer_id = ua.id)
        WHERE ua.result_answer_id = :raid', array(':raid' => $this->result_answer_id));
      while ($res_o = $res->fetch()) {
        $this->user_answer_ids[] = $res_o->answer_id;
      }
    }
  }

  /**
   * Implementation of save
   *
   * @see QuizQuestionResponse#save()
   */
  public function save() {
    $this->delete();

    $user_answer_id = db_insert('quiz_multichoice_user_answers')
      ->fields(array(
        'result_answer_id' => $this->result_answer_id,
        'choice_order' => $this->choice_order
      ))
      ->execute();

    $query = db_insert('quiz_multichoice_user_answer_multi')
      ->fields(array('user_answer_id', 'answer_id'));
    for ($i = 0; $i < count($this->user_answer_ids); $i++) {
      if ($this->user_answer_ids[$i]) {
        $query->values(array($user_answer_id, $this->user_answer_ids[$i]));
      }
    }
    $query->execute();
  }

  /**
   * Implementation of delete
   *
   * @see QuizQuestionResponse#delete()
   */
  public function delete() {
    $user_answer_id = array();
    $query = db_query('SELECT id FROM {quiz_multichoice_user_answers} WHERE result_answer_id = :raid', array(':raid' => $this->result_answer_id));
    while ($user_answer = $query->fetch()) {
      $user_answer_id[] = $user_answer->id;
    }

    if (!empty($user_answer_id)) {
      db_delete('quiz_multichoice_user_answer_multi')
        ->condition('user_answer_id', $user_answer_id, 'IN')
        ->execute();
    }

    db_delete('quiz_multichoice_user_answers')
      ->condition('result_answer_id', $this->result_answer_id)
      ->execute();
  }

  /**
   * Implementation of score
   *
   * @return uint
   *
   * @see QuizQuestionResponse#score()
   */
  public function score() {
    if ($this->question->choice_boolean || $this->isAllWrong()) {
      $score = $this->getMaxScore();
      foreach ($this->question->alternatives as $key => $alt) {
        if (in_array($alt['id'], $this->user_answer_ids)) {
          if ($alt['score_if_chosen'] <= $alt['score_if_not_chosen']) {
            $score = 0;
          }
        }
        else {
          if ($alt['score_if_chosen'] > $alt['score_if_not_chosen']) {
            $score = 0;
          }
        }
      }
    }
    else {
      $score = 0;
      foreach ($this->question->alternatives as $key => $alt) {
        if (in_array($alt['id'], $this->user_answer_ids)) {
          $score += $alt['score_if_chosen'];
        }
        else {
          $score += $alt['score_if_not_chosen'];
        }
      }
    }
    return $score;
  }

  /**
   * If all answers in a question is wrong
   *
   * @return boolean
   *  TRUE if all answers are wrong. False otherwise.
   */
  public function isAllWrong() {
    foreach ($this->question->alternatives as $key => $alt) {
      if ($alt['score_if_chosen'] > 0 || $alt['score_if_not_chosen'] > 0) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Implementation of getResponse
   *
   * @return answer
   *
   * @see QuizQuestionResponse#getResponse()
   */
  public function getResponse() {
    return $this->user_answer_ids;
  }

  /**
   * Returns a renderable array of question feedback.
   */
  public function getFeedback() {
    $out = array();
    $node = node_load($this->question->nid, $this->question->vid);
    $entity_info = entity_get_info('node');
    foreach ($entity_info['view modes'] as $view_mode => $info) {
      if ($this->canReview("quiz_question_view_" . $view_mode)) {
        node_build_content($node, $view_mode);
        unset($node->content['answers']);
        unset($node->content['links']);
        $out['question'][] = $node->content;
      }
    }

    $rows = array();

    $labels = array(
      'attempt' => t('Your answer'),
      'choice' => t('Choice'),
      'correct' => t('Correct?'),
      'score' => t('Score'),
      'answer_feedback' => t('Feedback'),
      'solution' => t('Correct answer'),
    );
    drupal_alter('quiz_feedback_labels', $labels);

    foreach ($this->getFeedbackValues() as $idx => $row) {
      foreach ($labels as $reviewType => $label) {
        if ((isset($row[$reviewType]) && $this->canReview($reviewType))) {
          $rows[$idx][$reviewType] = $row[$reviewType];
        }
      }
    }

    if ($this->isEvaluated()) {
      $score = $this->getScore();
      if ($this->isCorrect()) {
        $class = 'q-correct';
      }
      else {
        $class = 'q-wrong';
      }
    }
    else {
      $score = t('?');
      $class = 'q-waiting';
    }

    if ($this->canReview('score') || quiz_access_to_score()) {
      $out['score_display']['#markup'] = theme('quiz_question_score', array('score' => $score, 'max_score' => $this->getMaxScore(), 'class' => $class));
      $out['score_display']['#data'] = array(
        'score' => $score,
        'max_score' => $this->getMaxScore(),
        'class' => $class,
      );
    }

    if ($rows) {
      $headers = array_intersect_key($labels, $rows[0]);
      $type = $this->getQuizQuestion()->node->type;
      $out['response']['#markup'] = theme('quiz_question_feedback__' . $type, array('labels' => $headers, 'data' => $rows));
      $out['response']['#data'] = $rows;
      $out['response']['#labels'] = $labels;
    }

    if ($this->canReview('question_feedback')) {
      if ($properties = entity_load('quiz_question', FALSE, array('nid' => $this->quizQuestion->node->nid, 'vid' => $this->quizQuestion->node->vid))) {
        $quiz_question = reset($properties);
        $out['question_feedback']['#markup'] = check_markup($quiz_question->feedback, $quiz_question->feedback_format);
      }
    }

    if ($this->canReview('score')) {
      $out['max_score'] = array(
        '#type' => 'value',
        '#value' => $this->getMaxScore(),
      );
    }

    return $out;
  }

  /**
   * Implementation of getReportFormResponse
   */
  public function getFeedbackValues() {
    $this->orderAlternatives($this->question->alternatives);
    $simple_scoring = $this->question->choice_boolean;

    $data = array();
    foreach ($this->question->alternatives as $alternative) {
      $chosen = in_array($alternative['id'], $this->user_answer_ids);
      $not = $chosen ? '' : 'not_';

      $data[] = array(
        'choice' => check_markup($alternative['answer']['value'], $alternative['answer']['format']),
        'attempt' => $chosen ? quiz_icon('selected') : '',
        'correct' => $chosen ? $alternative['score_if_chosen'] > 0 ? quiz_icon('correct') : quiz_icon('incorrect') : '',
        'score' => $alternative["score_if_{$not}chosen"],
        'answer_feedback' => check_markup($alternative["feedback_if_{$not}chosen"]['value'], $alternative["feedback_if_{$not}chosen"]['format'], FALSE),
        'question_feedback' => 'Question feedback',
        'solution' => $alternative['score_if_chosen'] > 0 ? quiz_icon('should') : ($simple_scoring ? quiz_icon('should-not') : ''),
        'quiz_feedback' => "Quiz feedback",
      );
    }

    return $data;
  }

  /**
   * Order the alternatives according to the choice order stored in the database
   *
   * @param array $alternatives
   *  The alternatives to be ordered
   */
  protected function orderAlternatives(array &$alternatives) {
    if (!$this->question->choice_random) {
      return;
    }
    $result = db_query('SELECT choice_order FROM {quiz_multichoice_user_answers}
            WHERE result_answer_id = :raid', array(':raid' => $this->result_answer_id))->fetchField();
    if (!$result) {
      return;
    }
    $order = explode(',', $result);
    $newAlternatives = array();
    foreach ($order as $value) {
      foreach ($alternatives as $alternative) {
        if ($alternative['id'] == $value) {
          $newAlternatives[] = $alternative;
          break;
        }
      }
    }
    $alternatives = $newAlternatives;
  }

}
