<?php

/**
 * Helpers.
 */
class EntityTranslationDomainAccess {

  /**
   * Checks if user is allowed to perform an operation on an entity translation.
   *
   * @param string $entity_type
   * @param object $entity
   * @param string $translation_langcode
   * @param string $op
   *   An operation to be performed on an entity translation. Could be:
   *   - "add"
   *   - "edit"
   *   - "delete"
   *   - "any" (this one is special, it's used when it's not clear what
   *     operation will be performed)
   * @param object|null $account
   *   User account to check. If not provided, current user will be used.
   *
   * @return bool
   */
  public static function checkTranslationAccess($entity_type, $entity, $translation_langcode, $op, $account = NULL) {
    return user_access('bypass entity translation domain access', $account) || in_array($translation_langcode, self::getUserLangcodes($account));
  }

  /**
   * Stores an array of langcodes to be associated with a domain.
   *
   * @param int $domain_id
   * @param array $langcodes
   */
  public static function setDomainLangcodes($domain_id, $langcodes) {
    $domain_to_languages_map = variable_get('entity_translation_domain_access_domain_to_languages_map', array());
    $domain_to_languages_map[$domain_id] = $langcodes;
    variable_set('entity_translation_domain_access_domain_to_languages_map', $domain_to_languages_map);
  }

  /**
   * Returns an array of langcodes associated with a domain.
   *
   * @param int $domain_id
   *
   * @return array
   */
  public static function getDomainLangcodes($domain_id) {
    $domain_to_languages_map = variable_get('entity_translation_domain_access_domain_to_languages_map', array());
    return isset($domain_to_languages_map[$domain_id]) ? $domain_to_languages_map[$domain_id] : array();
  }

  /**
   * Returns an array of langcodes allowed for a user.
   *
   * @param object|null $account
   *   User account to check. If not provided, current user will be used.
   *
   * @return array
   */
  public static function getUserLangcodes($account = NULL) {
    if (!isset($account)) {
      $account = $GLOBALS['user'];
    }
    if (!isset($account->domain_user)) {
      $account = user_load($account->uid);
    }
    $langcodes = array();
    foreach ($account->domain_user as $domain_id) {
      $langcodes = array_merge($langcodes, self::getDomainLangcodes($domain_id));
    }
    return array_unique($langcodes);
  }
}
