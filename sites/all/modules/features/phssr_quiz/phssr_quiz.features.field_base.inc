<?php
/**
 * @file
 * phssr_quiz.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function phssr_quiz_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_quiz_type'
  $field_bases['field_quiz_type'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_quiz_type',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'Quiz Type A' => 'Quiz Type A',
        'Quiz Type B' => 'Quiz Type B',
        'Quiz Type C' => 'Quiz Type C',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  return $field_bases;
}
