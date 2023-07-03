<?php

/**
 * @file
 * Views handler.
 */

/**
 * Field handler to present font enable/disable link.
 */
class views_handler_field_fontyourface_enabled_yes_no extends views_handler_field {

  function construct() {

    parent::construct();
    $this->additional_fields['enabled'] = 'enabled';

  }

  function query() {

    $this->ensure_my_table();
    $this->add_additional_fields();

  }

  function render($values) {

    $enabled = $this->get_value($values, 'enabled');

    // This is intentionally not translated, so it can be used in class names.

    if ($enabled) {

      return 'yes';

    }
    else {

      return 'no';

    }

  }

}
