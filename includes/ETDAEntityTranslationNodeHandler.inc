<?php

/**
 * @file
 *  Translation handler for nodes.
 */

class ETDAEntityTranslationNodeHandler extends EntityTranslationNodeHandler {

  /**
   * @see EntityTranslationDefaultHandler::getTranslationAccess()
   */
  public function getTranslationAccess($langcode) {
    // Make sure first that the user can has the domain based access.
    if (locale_multilingual_node_type($this->entity->type) && !EntityTranslationDomainAccess::checkTranslationAccess($this->entityType, $this->entity, $langcode, 'any')) {
      return FALSE;
    }
    return parent::getTranslationAccess($langcode);
  }

  /**
   * @see EntityTranslationNodeHandler::entityForm()
   */
  public function entityForm(&$form, &$form_state) {
    // Trick: this will trigger the appearance of the 'Delete translation'
    // button, because that button will be shown only if the 'delete' button is
    // already in the form, but this will not be the case because the user does
    // not always have the permission to delete the node.

    // @todo: we should put some conditions here maybe.
    $form['actions']['delete'] = array();
    return parent::entityForm($form, $form_state);
  }
}
