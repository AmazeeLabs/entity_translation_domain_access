<?php

/**
 * Overrides the node "Delete link" handler.
 */
class etda_views_handler_field_node_link_delete extends views_handler_field_node_link_delete {

  /**
   * {@inheritdoc}
   */
  function render_link($node, $values) {
    if (!EntityTranslationDomainAccess::checkNodeActionAccess($node, 'delete')) {
      return NULL;
    }
    return parent::render_link($node, $values);
  }
}
