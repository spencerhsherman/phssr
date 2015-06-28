<?php
/**
 * @file
 * phssr_quiz.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function phssr_quiz_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'quiz_admin_review_options_end';
  $strongarm->value = array(
    'quiz_question_view_full' => 'quiz_question_view_full',
    'quiz_question_view_teaser' => 0,
    'attempt' => 'attempt',
    'choice' => 'choice',
    'correct' => 'correct',
    'score' => 'score',
    'answer_feedback' => 'answer_feedback',
    'question_feedback' => 'question_feedback',
    'solution' => 'solution',
    'quiz_feedback' => 'quiz_feedback',
  );
  $export['quiz_admin_review_options_end'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'quiz_admin_review_options_question';
  $strongarm->value = array(
    'quiz_question_view_full' => 'quiz_question_view_full',
    'quiz_question_view_teaser' => 0,
    'attempt' => 'attempt',
    'choice' => 'choice',
    'correct' => 'correct',
    'score' => 'score',
    'answer_feedback' => 'answer_feedback',
    'question_feedback' => 'question_feedback',
    'solution' => 'solution',
    'quiz_feedback' => 'quiz_feedback',
  );
  $export['quiz_admin_review_options_question'] = $strongarm;

  return $export;
}
