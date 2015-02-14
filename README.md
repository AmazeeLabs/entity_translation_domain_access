# Entity translation domain access

A Drupal module that provides entity translation permissions basing on the domains assigned to a user.

## How it works

The Domain Access module provides the "Domain access settings" on the user edit form. This module adds the "Entity translation languages" tab to the domain pages (admin/structure/domain/view/%domain/entity-translation). Using this settings the module restricts access to the entity translation pages.

For example, Anna is assigned to the Switzerland and France domains. The "Entity translation languages" for the Switzerland domain are DE-CH and FR-CH. For the France domain it is FR. So, Anna will only be allowed to edit DE-CH, FR-CH, and FR translations of any entities.
