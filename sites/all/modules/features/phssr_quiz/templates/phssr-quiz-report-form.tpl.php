<?php
/**
 * @file
 * Themes the question report
 *
 */
/*
 * Available variables:
 * $form - FAPI array. You can print this entire page with drupal_render_children($form) and the
 *         default styling will be used.
 *
 * All questions are in $questions[x] where x is an integer.
 * Useful values:
 *
 * $questions[x]['nid'] - The Drupal Node NID for the Question node.
 * $questions[x]['result_id'] - The key value used in results specific database tables.
 *
 * $questions[x]['response']['#markup'] - Rendered HTML for the responses table derived from theme_quiz_question_feedback().
 * $questions[x]['response']['#data'][x] - Row data passed to theme_table() that represents each row of data in the table.
 * $questions[x]['response']['#labels'][x] - Header data passed to theme_table() that represents the header row of a table.
 *
 * $questions[x]['question'] - Render array for the actual Question text. Use drupal_render() to display.
 *
 * $questions[x]['max_score'] - Integer value for the max possible score.
 *
 * $questions[x]['question_feedback'] - ?
 *
 * $questions[x]['score_display']['#markup'] - Rendered HTML derived from theme_quiz_question_score().
 * $questions[x]['score_display']['#data']['score'] - Integer value score for this question.
 * $questions[x]['score_display']['#data']['max_score] - Integer value of the max possible score for this question.
 * $questions[x]['score_display']['#data']['class'] - String value of the CSS class value passed to theme_quiz_question_score().
 *
 *
 */

/**
 * Hide Form Elements that are being "pretty" rendered.
 * Do not remove this, or an ugly version of the question results
 * will display below your themed version.
 */
if (isset($hide_form_element)) {
  foreach ($hide_form_element as $key) {
    hide($form[$key]);
  }
}
?>


<h2><?php print t('Question results'); ?></h2>
<div class="quiz-report">
  <?php foreach($questions as $question) : ?>
    <div class="quiz-report-row clearfix">
      <div class="quiz-report-question dt">
        <div class="quiz-report-question-header clearfix">
          <?php print drupal_render($question['score_display']); ?>
          <h3><?php print t('Question') ?></h3>
        </div>
        <?php print drupal_render($question['question']); ?>
      </div>
      <?php if (isset($question['response'])): ?>
        <div class="quiz-report-response dd">
          <h3 class="quiz-report-response-header"><?php print t('Response') ?></h3>
          <?php //print drupal_render($question['response']); ?>
          <table class="table table-striped sticky-enabled">
            <thead>
            <tr>
              <?php foreach($question['response']['#labels'] as $label) : ?>
                <th><?php print $label; ?></th>
              <?php endforeach; ?>
            </tr>
            </thead>
            <tbody>
            <?php foreach($question['response']['#data'] as $key => $data) : ?>
              <tr>
                <?php foreach ($data as $class => $value) : ?>
                  <td class="<?php print $class; ?>"><?php print $value; ?></td>
                <?php endforeach; ?>
              </tr>
            <?php endforeach; ?>
            </tbody>
          </table>
        </div>
      <?php endif; ?>
      <div class="quiz-report-question-feedback dd">
        <?php print drupal_render($question['question_feedback']); ?>
      </div>
      <div class="quiz-report-score-feedback dd">
        <?php print drupal_render($question['score']); ?>
        <?php print drupal_render($question['answer_feedback']); ?>
      </div>
    </div>
  <?php endforeach; ?>
  <div class="quiz-report-quiz-feedback dd">
    <?php if (isset($form['quiz_feedback']) && $form['quiz_feedback']['#markup']): ?>
      <h2><?php print t('Quiz feedback'); ?></h2>
      <?php print drupal_render($form['quiz_feedback']); ?>
    <?php endif; ?>
  </div>
</div>
<div class="quiz-score-submit">
  <?php print drupal_render_children($form); ?>
</div>