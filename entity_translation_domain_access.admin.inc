<?php

/**
 * Admin form callback.
 */
function entity_translation_domain_access_admin_form($form, &$form_state, $domain) {
  $options = language_list();
  foreach ($options as $langcode => $language) {
    $options[$langcode] = $language->name;
  }
  $form['languages'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Languages'),
    '#description' => t('Users, assigned to the @domain domain, will be allowed to add/edit/delete entity translations for the selected languages.', array('@domain' => $domain['sitename'])),
    '#options' => $options,
    '#default_value' => EntityTranslationDomainAccess::getDomainLangcodes($domain['domain_id']),
  );
  $form['#domain_id'] = $domain['domain_id'];
  $form['action'] = array('#type' => 'actions');
  $form['action']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Submit callback for the admin form.
 */
function entity_translation_domain_access_admin_form_submit(&$form, $form_state) {
  EntityTranslationDomainAccess::setDomainLangcodes($form['#domain_id'], array_filter($form_state['values']['languages']));
  drupal_set_message(t('The configuration options have been saved.'));
}
