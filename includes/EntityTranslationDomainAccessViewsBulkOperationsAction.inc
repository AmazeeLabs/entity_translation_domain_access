<?php

/**
 * Overrides the VBO action class.
 *
 * We can't use node/entity access system because there is no "unpublish"
 * operation.
 */
class EntityTranslationDomainAccessViewsBulkOperationsAction extends ViewsBulkOperationsAction {

  /**
   * {@inheritdoc}
   */
  public function execute($data, array $context) {
    if ($this->entityType == 'node') {
      $node = $data;
      switch ($this->operationId) {
        case 'action::node_unpublish_action':
          if (!empty($node->status) && !EntityTranslationDomainAccess::checkNodeActionAccess($node, 'unpublish')) {
            drupal_set_message(t('The content %title has not been unpublished because either it assigned to domains that you are not assigned to, or it has translations you do not have access to.', array('%title' => entity_label('node', $node))), 'warning');
            return;
          }
          break;

        case 'action::views_bulk_operations_delete_item':
          if (!EntityTranslationDomainAccess::checkNodeActionAccess($node, 'delete')) {
            drupal_set_message(t('The content %title has not been deleted because either it assigned to domains that you are not assigned to, or it has translations you do not have access to.', array('%title' => entity_label('node', $node))), 'warning');
            return;
          }
          break;
      }
    }
    parent::execute($data, $context);
  }
}
