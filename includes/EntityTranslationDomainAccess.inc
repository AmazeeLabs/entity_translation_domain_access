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
   * Checks if user is allowed to perform an action on a node.
   *
   * @param object $node
   * @param string $action
   *   An action to be performed on a node. Could be:
   *   - "unpublish"
   *   - "delete"
   * @param object|null $account
   *   User account to check. If not provided, current user will be used.
   *
   * @return bool
   */
  public static function checkNodeActionAccess($node, $action, $account = NULL) {
    if (user_access('bypass entity translation domain access', $account)) {
      return TRUE;
    }
    if (!isset($account)) {
      $account = $GLOBALS['user'];
    }
    if (!isset($account->domain_user)) {
      $account = user_load($account->uid);
    }
    if (!array_diff($node->domains, $account->domain_user)) {
      $handler = entity_translation_get_handler('node', $node);
      $translation_langcodes = array_keys($handler->getTranslations()->data);
      if (!array_diff($translation_langcodes, self::getUserLangcodes($account))) {
        return TRUE;
      }
    }
    return FALSE;
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
    $cache =& drupal_static(__FUNCTION__, array());
    if (!isset($cache[$account->uid])) {
      if (!isset($account->domain_user)) {
        $account = user_load($account->uid);
      }
      $langcodes = array();
      foreach ($account->domain_user as $domain_id) {
        $langcodes = array_merge($langcodes, self::getDomainLangcodes($domain_id));
      }
      $cache[$account->uid] = array_unique($langcodes);
    }
    return $cache[$account->uid];
  }
}
