<?php
/**
 * @file
 * phssr_quiz.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function phssr_quiz_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-quiz-field_quiz_type'
  $field_instances['node-quiz-field_quiz_type'] = array(
    'bundle' => 'quiz',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_quiz_type',
    'label' => 'Quiz Type',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 2,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Quiz Type');

  return $field_instances;
}
