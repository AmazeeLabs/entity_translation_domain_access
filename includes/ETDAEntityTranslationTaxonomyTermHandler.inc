<?php

/**
 * @file
 *  Taxonomy term translation handler.
 */

class ETDAEntityTranslationTaxonomyTermHandler extends EntityTranslationTaxonomyTermHandler {

  /**
   * @see EntityTranslationDefaultHandler::getTranslationAccess()
   */
  public function getTranslationAccess($langcode) {
    // Make sure first that the user can has the domain based access.
    if (!EntityTranslationDomainAccess::checkTranslationAccess($this->entityType, $this->entity, $langcode, 'any')) {
      return FALSE;
    }
    return parent::getTranslationAccess($langcode);
  }
}
