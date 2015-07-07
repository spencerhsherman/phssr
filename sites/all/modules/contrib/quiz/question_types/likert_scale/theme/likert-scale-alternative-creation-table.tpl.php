<?php

/**
 * @file
 * Handles the layout of the choice creation form.
 *
 *
 * Variables available:
 * - $form
 */
foreach (element_children($form) as $key) {
  if (is_numeric($key)) {
    $row = array(
      'data' => array(
        drupal_render($form[$key]['answer']) . drupal_render($form[$key]['advanced']),
        drupal_render($form[$key]['weight']),
      ),
      'class' => array('draggable'),
    );
    $rows[] = $row;
  }
}
print theme('table', array('rows' => $rows, 'header' => array(t('Answer'), t('Weight')), 'attributes' => array('id' => 'likert-scale-alternatives-table')));
print drupal_render_children($form);
