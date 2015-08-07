<?php

/**
 * Views node edit link with ETDA support.
 */
class entity_translation_domain_access_views_handler_field_node_link_edit extends views_handler_field_node_link {

  /**
   * Renders the link.
   */
  function render_link($node, $values) {
    if (!node_access('update', $node)) {
      return NULL;
    }
    if (!EntityTranslationDomainAccess::checkTranslationAccess('node', $node, $GLOBALS['language']->language, 'edit')) {
      return NULL;
    }
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "node/$node->nid/edit";
    $this->options['alter']['query'] = drupal_get_destination();
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    return $text;
  }

}
