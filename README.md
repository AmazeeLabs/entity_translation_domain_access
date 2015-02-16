# Entity translation domain access

A Drupal module that provides entity translation permissions basing on the domains assigned to a user.

## How it works

The Domain Access module provides the "Domain access settings" on the user edit form. This module adds the "Entity translation languages" tab to the domain pages (admin/structure/domain/view/%domain/entity-translation). Using this settings the module restricts access to the entity translation pages using the {user-account}-{user-domains}-{domain-languages} chain.

For example, Anna is assigned to the Switzerland and France domains. The "Entity translation languages" for the Switzerland domain are DE-CH and FR-CH. For the France domain it is FR.

### All entities: translations access control

Anna will only be allowed to add/edit/delete DE-CH, FR-CH, and FR translations.

### Content (node) entities: delete/unpublish access control

Anna will not be allowed to delete or unpublish a content which either

- is assigned to domains to which Anna is not assigned too (for example, if content is assigned to the USA domain)
- has translations which Anna can't delete (for example, if content is translated to EN-US language)

### Content (node) entities: domain assignment control

Anna will only be able to assign/unassign content to domains to which she is assigned. So, only Switzerland and France domains. She can't change other content domains.

### Permissions

Roles that should not be constrained by the above rules, should have the "Bypass entity translation domain access" permission. The module does not respect the "Bypass content access control" permission.
