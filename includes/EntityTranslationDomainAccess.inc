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
    if (user_access('bypass entity translation domain access', $account)) {
      return TRUE;
    }
    if ($op == 'edit' || $op == 'delete') {
      $handler = entity_translation_get_handler($entity_type, $entity);
      $translations = $handler->getTranslations();
      if (!isset($translations->data[$translation_langcode])) {
        // We can't edit/delete non-existing translation.
        return FALSE;
      }
    }
    return in_array($translation_langcode, self::getUserLangcodes($account));
  }

  /**
   * Checks if an user can translate in general to a specific language.
   *
   * @param string $langcode
   *  The language code.
   *
   * @param object $account
   *  Optional, an user object to check.
   */
  public static function canTranslateToLanguage($langcode, $account = NULL) {
    return user_access('bypass entity translation domain access', $account) || in_array($langcode, self::getUserLangcodes($account));
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
    // @todo: we should statically cache the results here.
    if (user_access('bypass entity translation domain access', $account)) {
      return TRUE;
    }
    $node_domains = !empty($node->domains) ? array_filter($node->domains) : array();
    if (!array_diff($node_domains, self::getUserDomains($account))) {
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
   * Returns an array of domain IDs assigned to the user account.
   *
   * @param object|null $account
   *   User account. If not provided, current user will be used.
   *
   * @return array
   */
  public static function getUserDomains($account = NULL) {
    if (!isset($account)) {
      $account = $GLOBALS['user'];
    }
    $cache =& drupal_static(__CLASS__ . '::' . __METHOD__, array());
    if (!isset($cache[$account->uid])) {
      if (!isset($account->domain_user)) {
        $account = user_load($account->uid);
      }
      $cache[$account->uid] = empty($account->domain_user) ? array() : $account->domain_user;
    }
    return $cache[$account->uid];
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
    $langcodes = array();
    foreach (self::getUserDomains($account) as $domain_id) {
      $langcodes = array_merge($langcodes, self::getDomainLangcodes($domain_id));
    }
    return array_unique($langcodes);
  }

}
