<?php

/**
 * @file
 * API functions.
 */

define('FONTS_COM_API_BASE_URL', 'https://api.fonts.com');
define('FONTS_COM_API_APP_KEY', '1fdb130c-d5c0-4fab-8e2b-271508570323932606');

/**
 * Gets list of fonts in a given project.
 */
function fonts_com_get_fonts_in_project($project_id) {

  $fonts = array();

  $path = '/rest/json/Fonts/?wfspstart=0&wfsplimit=100&wfspid=' . $project_id;

  $response = backdrop_http_request(FONTS_COM_API_BASE_URL . $path, array('headers' => fonts_com_api_headers($path)));

  if ($response->code == 200) {

    $data = json_decode($response->data);

    if (isset($data->Fonts) && isset($data->Fonts->Font)) {

      $font = $data->Fonts->Font;
      $fonts = fonts_com_unknown_to_array($font);

    }

  }
  else {

    backdrop_set_message(t('There was an error importing a project (@project) font list from Fonts.com.', array('@project' => $project_id)), 'error');
    fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

  }

  return $fonts;

}

/**
 * Gets a list of all fonts, in given range.
 */
function fonts_com_get_all_fonts($start = 0, $limit = 100, $filters = array()) {

  $result = array(
    'fonts' => array(),
    'count' => FALSE,
  );

  if (
    (isset($filters['wfsfree'])) &&
    ($filters['wfsfree'] == 0)
  ) {
    $filters['wfsfree'] = 'true';
  }
  if (
    (isset($filters['wfsfree'])) &&
    ($filters['wfsfree'] == -1)
  ) {
    $filters['wfsfree'] = 'false';
  }

  $query = array(
    'wfspstart' => $start,
    'wfsplimit' => $limit,
  ) + $filters;

  $path = '/rest/json/AllFonts/?' . backdrop_http_build_query($query);

  $response = backdrop_http_request(FONTS_COM_API_BASE_URL . $path, array('headers' => fonts_com_api_headers($path)));

  if ($response->code == 200) {

    $data = json_decode($response->data);

    $font = $data->AllFonts->Font;
    $result['fonts'] = fonts_com_unknown_to_array($font);
    $result['count'] = intval($data->AllFonts->TotalRecords);

  }
  else {

    backdrop_set_message(t('There was an error importing a font list from Fonts.com. Check !configuration.', array('!configuration' => l(t('configuration'), 'admin/config/user-interface/fontyourface/fonts_com'))), 'error');
    fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

  }

  return $result;

}

/**
 * Gets total font count.
 */
function fonts_com_get_all_fonts_count() {

  $path = '/rest/json/AllFonts/?wfspstart=0&wfsplimit=1';

  $response = backdrop_http_request(FONTS_COM_API_BASE_URL . $path, array('headers' => fonts_com_api_headers($path)));

  if ($response->code == 200) {

    $data = json_decode($response->data);
    return $data->AllFonts->TotalRecords;

  }

  backdrop_set_message(t('There was an error importing the font count from Fonts.com. Check !configuration.', array('!configuration' => l(t('configuration'), 'admin/config/user-interface/fontyourface/fonts_com'))), 'error');
  fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

  return FALSE;

}

/**
 * Gets auth key for a given account.
 */
function fonts_com_get_key_for_account($email, $password) {

  $key = fonts_com_get_existing_key_for_account($email, $password);

  if ($key) return $key;

  $path = '/rest/json/Accounts/?wfsemail=' . $email;

  $response = backdrop_http_request(FONTS_COM_API_BASE_URL . $path, array('headers' => array('AppKey' => FONTS_COM_API_APP_KEY, 'Password' => $password)));

  if ($response->code == 200) {

    $data = json_decode($response->data);

    if (
      isset($data->Accounts) &&
      isset($data->Accounts->Account) &&
      isset($data->Accounts->Account->AuthorizationKey)
    ) {

      $key = $data->Accounts->Account->AuthorizationKey;

    }
    else {

      backdrop_set_message(t('The submitted email and password were not recognized by Fonts.com.'), 'error');
      fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

    }

  }
  else {

    backdrop_set_message(t('There was an error getting your key from Fonts.com.'), 'error');
    fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

  }

  return $key;

}

/**
 * Gets auth key for a given account.
 */
function fonts_com_get_existing_key_for_account($email, $password) {

  $path = '/rest/json/GetToken/?wfsemail=' . $email;

  $response = backdrop_http_request(FONTS_COM_API_BASE_URL . $path, array('headers' => array('AppKey' => FONTS_COM_API_APP_KEY, 'Password' => $password)));

  if ($response->code == 200) {

    $data = json_decode($response->data);

    if (
      isset($data->Accounts) &&
      isset($data->Accounts->Account) &&
      isset($data->Accounts->Account->AuthorizationKey)
    ) {

      return $data->Accounts->Account->AuthorizationKey;

    }

  }

  return FALSE;

}

/**
 * Creates a new account.
 */
function fonts_com_create_account($email, $first_name, $last_name) {

  $success = FALSE;

  $path = '/rest/json/Accounts/';

  $response = backdrop_http_request(FONTS_COM_API_BASE_URL . $path, array('headers' => array('AppKey' => FONTS_COM_API_APP_KEY), 'method' => 'POST', 'data' => 'wfsfirst_name=' . $first_name . '&wfslast_name=' . $last_name . '&wfsemail=' . $email));

  if ($response->code == 200) {

    $data = json_decode($response->data);

    if (
      isset($data->Accounts) &&
      isset($data->Accounts->Message) &&
      ($data->Accounts->Message == 'Success')
    ) {

      $success = TRUE;

    }
    else {

      backdrop_set_message(t('Fonts.com as unable to create your account.'), 'error');
      fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

    }

  }
  else {

    backdrop_set_message(t('There was an error creating an account at Fonts.com.'), 'error');
    fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

  }

  return $success;

}

/**
 * Gets list of domains in a given project.
 */
function fonts_com_get_domains_in_project($project_id) {

  $domains = array();

  $path = '/rest/json/Domains/?wfspstart=0&wfsplimit=100&wfspid=' . $project_id;

  $response = backdrop_http_request(FONTS_COM_API_BASE_URL . $path, array('headers' => fonts_com_api_headers($path)));

  if ($response->code == 200) {

    $data = json_decode($response->data);

    if ($data->Domains->TotalRecords > 0) {

      $domain = $data->Domains->Domain;
      $domains = fonts_com_unknown_to_array($domain);

    }

  }
  else {

    backdrop_set_message(t('There was an error importing a project (@project) domain list from Fonts.com.', array('@project' => $project_id)), 'error');
    fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

  }

  return $domains;

}

/**
 * Saves list of domains in a given project.
 */
function fonts_com_save_domains_in_project($project_id, $new_domains) {

  $old_domains = array();
  $domain_to_index = array();

  $old_domain_objects = fonts_com_get_domains_in_project($project_id);

  foreach ($old_domain_objects as $index => $old_domain_object) {

    $domain_to_index[$old_domain_object->DomainName] = $index;
    $old_domains[] = $old_domain_object->DomainName;

  }

  $add_domains = array_diff($new_domains, $old_domains);
  $delete_domains = array_diff($old_domains, $new_domains);

  foreach ($delete_domains as $delete_domain) {

    $delete_domain_object = $old_domain_objects[$domain_to_index[$delete_domain]];
    fonts_com_delete_domain($project_id, $delete_domain_object->DomainID);

  }

  foreach ($add_domains as $add_domain) {
    fonts_com_add_domain($project_id, $add_domain);
  }

  fonts_com_publish();

}

/**
 * Deletes a single domain in a project.
 */
function fonts_com_delete_domain($project_id, $domain_id) {

  $path = '/rest/json/Domains/?wfspid=' . $project_id . '&wfsdomain_id=' . $domain_id;

  $response = backdrop_http_request(FONTS_COM_API_BASE_URL . $path, array('headers' => fonts_com_api_headers($path), 'method' => 'DELETE'));

  if ($response->code == 200) {
    return TRUE;
  }
  else {

    backdrop_set_message(t('There was an error deleting a domain (@domain) in a project (@project) on Fonts.com.', array('@domain' => $domain_id, '@project' => $project_id)), 'error');
    fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

  }

  return FALSE;

}

/**
 * Adds a single domain to a project.
 */
function fonts_com_add_domain($project_id, $domain_name) {

  $path = '/rest/json/Domains/?wfspid=' . $project_id;

  $response = backdrop_http_request(FONTS_COM_API_BASE_URL . $path, array('headers' => fonts_com_api_headers($path), 'method' => 'POST', 'data' => 'wfsdomain_name=' . $domain_name));

  if ($response->code == 200) {
    return TRUE;
  }
  else {

    backdrop_set_message(t('There was an error adding a domain (@domain) in a project (@project) on Fonts.com.', array('@domain' => $domain_name, '@project' => $project_id)), 'error');
    fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

  }

  return FALSE;

}

/**
 * Gets list of projects from API.
 */
function fonts_com_get_projects() {

  $projects = array();

  $path = '/rest/json/Projects/?wfspstart=0&wfsplimit=100';

  $response = backdrop_http_request(FONTS_COM_API_BASE_URL . $path, array('headers' => fonts_com_api_headers($path)));

  if ($response->code == 200) {

    $data = json_decode($response->data);

    if ($data->Projects->TotalRecords > 0) {

      $project = $data->Projects->Project;
      $projects = fonts_com_unknown_to_array($project);

    }

  }
  else {

    backdrop_set_message(t('There was an error importing project list from Fonts.com. Check !configuration.', array('!configuration' => l(t('configuration'), 'admin/config/user-interface/fontyourface/fonts_com'))), 'error');
    fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

  }

  return $projects;

}

/**
 * Gets list of projects from API.
 */
function fonts_com_get_project_by_id($project_id) {

  $projects = fonts_com_get_projects();

  if (count($projects) > 0) {

    foreach ($projects as $project) {

        if ($project->ProjectKey == $project_id) {
          return $project;
        }

      }

  }

  return FALSE;

}

/**
 * Update.
 */
function fonts_com_save_project(&$project) {

  $path = '/rest/json/Projects/';
  $method = 'POST';

  if (isset($project->ProjectKey)) {

    $path .= '?wfspid=' . $project->ProjectKey;
    $method = 'PUT';

  }

  $response = backdrop_http_request(FONTS_COM_API_BASE_URL . $path, array('headers' => fonts_com_api_headers($path), 'method' => $method, 'data' => 'wfsproject_name=' . $project->ProjectName));

  if ($response->code == 200) {

    $data = json_decode($response->data);

    $projects = fonts_com_unknown_to_array($data->Projects->Project);

    foreach ($projects as $response_project) {

      if ($response_project->ProjectName == $project->ProjectName) {
        $project->ProjectKey = $response_project->ProjectKey;
      }

    }

    return TRUE;

  }
  else {

    backdrop_set_message(t('There was an error saving project to Fonts.com.'), 'error');
    fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

  }

  return FALSE;

}

/**
 * Adds font to project.
 */
function fonts_com_add_font_to_project($font_id, $project_id) {

  $path = '/rest/json/Fonts/?wfspid=' . $project_id;

  $response = backdrop_http_request(FONTS_COM_API_BASE_URL . $path, array('headers' => fonts_com_api_headers($path), 'method' => 'POST', 'data' => 'wfsfid=' . $font_id));

  if ($response->code == 200) {

    $data = json_decode($response->data);
    fonts_com_publish();
    return TRUE;

  }
  else {

    backdrop_set_message(t('There was an error enabling the font on Fonts.com.'), 'error');
    fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

  }

  return FALSE;

}

/**
 * Deletes font from project.
 */
function fonts_com_delete_font_from_project($font_id, $project_id) {

  $path = '/rest/json/Fonts/?wfspid=' . $project_id . '&wfsfid=' . $font_id;

  $response = backdrop_http_request(FONTS_COM_API_BASE_URL . $path, array('headers' => fonts_com_api_headers($path), 'method' => 'DELETE'));

  if ($response->code == 200) {

    fonts_com_publish();
    return TRUE;

  }
  else {

    backdrop_set_message(t('There was an error disabling the font on Fonts.com.'), 'error');
    fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

  }

  return FALSE;

}

/**
 * Sets selector on font.
 */
function fonts_com_set_font_selector($project_id, $font_id, $selector) {

  fonts_com_add_selector_to_project($project_id, $selector);
  $project_selectors = fonts_com_get_selectors_in_project($project_id);
  $selector_id = FALSE;

  foreach ($project_selectors as $project_selector) {

    if ($project_selector->SelectorTag == $selector) {
      $selector_id = $project_selector->SelectorID;
    }
    elseif ($project_selector->SelectorFontID == $font_id) {
      fonts_com_delete_selector_in_product($project_id, $project_selector->SelectorID);
    }

  }

  if ($selector_id) {

    $path = '/rest/json/Selectors/?wfspid=' . $project_id;

    $response = backdrop_http_request(
      FONTS_COM_API_BASE_URL . $path,
      array(
        'headers' => fonts_com_api_headers($path),
        'method' => 'PUT',
        'data' => 'wfsselector_ids=' . $selector_id . '&wfsfont_ids=' . $font_id
      )
    );

    if ($response->code == 200) {

      fonts_com_publish();
      return TRUE;

    }
    else {

      backdrop_set_message(t('There was an error adding a CSS selector on Fonts.com.'), 'error');
      fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

    }

  }

  return FALSE;

}

/**
 * Removes selector from a project.
 */
function fonts_com_delete_selector_in_product($project_id, $selector_id) {

  $path = '/rest/json/Selectors/?wfspid=' . $project_id . '&wfsselector_id=' . $selector_id . '&wfsnopublish=1';

  $response = backdrop_http_request(
    FONTS_COM_API_BASE_URL . $path,
    array(
      'headers' => fonts_com_api_headers($path),
      'method' => 'DELETE',
    )
  );

  if ($response->code == 200) {

    return TRUE;

  }
  else {

    backdrop_set_message(t('There was an error removing a CSS selector on Fonts.com.'), 'error');
    fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

  }

  return FALSE;

}

/**
 * Adds selector to project.
 */
function fonts_com_add_selector_to_project($project_id, $selector) {

  $path = '/rest/json/Selectors/?wfspid=' . $project_id;

  $response = backdrop_http_request(
    FONTS_COM_API_BASE_URL . $path,
    array(
      'headers' => fonts_com_api_headers($path),
      'method' => 'POST',
      'data' => 'wfsselector_tag=' . urlencode($selector)
    )
  );

  if ($response->code == 200) {

    return TRUE;

  }
  else {

    backdrop_set_message(t('There was an error adding a CSS selector on Fonts.com.'), 'error');
    fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

  }

  return FALSE;

}

/**
 * Gets selectors in project.
 */
function fonts_com_get_selectors_in_project($project_id) {

  $selectors = array();

  $path = '/rest/json/Selectors/?wfspid=' . $project_id;

  $response = backdrop_http_request(
    FONTS_COM_API_BASE_URL . $path,
    array(
      'headers' => fonts_com_api_headers($path),
    )
  );

  if ($response->code == 200) {

    $data = json_decode($response->data);

    if (isset($data->Selectors) && isset($data->Selectors->Selector)) {

      $selectors = fonts_com_unknown_to_array($data->Selectors->Selector);

    }

  }
  else {

    backdrop_set_message(t('There was an error adding a CSS selector on Fonts.com.'), 'error');
    fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

  }

  return $selectors;

}

/**
 * Gets choices for filter.
 */
function fonts_com_get_filter_choices($filter) {

  $choices = array();

  $path = '/rest/json/FilterValues/?wfsfiltertype=' . $filter;

  $response = backdrop_http_request(
    FONTS_COM_API_BASE_URL . $path,
    array(
      'headers' => fonts_com_api_headers($path),
    )
  );

  if ($response->code == 200) {

    $data = json_decode($response->data);

    if (isset($data->FilterValues) && isset($data->FilterValues->FilterValue)) {

      $choices = fonts_com_unknown_to_array($data->FilterValues->FilterValue);

    }

  }
  else {

    backdrop_set_message(t('There was an error getting filter choices from Fonts.com. Check !configuration.', array('!configuration' => l(t('configuration'), 'admin/config/user-interface/fontyourface/fonts_com'))), 'error');
    fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

  }

  return $choices;

}

/**
 * Gets choices for all filter given current values.
 */
function fonts_com_get_all_filter_choices($current_filters) {

  $path = '/rest/json/AllFilterValues/?' . backdrop_http_build_query($current_filters);

  $response = backdrop_http_request(
    FONTS_COM_API_BASE_URL . $path,
    array(
      'headers' => fonts_com_api_headers($path),
    )
  );

  if ($response->code == 200) {

    $data = json_decode($response->data);

    if (isset($data->FilterValues) && isset($data->FilterValues->FilterValue)) {

      $choices = fonts_com_unknown_to_array($data->FilterValues->FilterValue);
      $choices_by_filter = array();

      foreach ($choices as $choice) {

        if (!isset($choices_by_filter[$choice->FilterType])) {
          $choices_by_filter[$choice->FilterType] = array();
        }

        $choices_by_filter[$choice->FilterType][] = $choice;

      }

      return $choices_by_filter;

    }
    else {

      backdrop_set_message(t('There was an error getting filter choices from Fonts.com. Check !configuration.', array('!configuration' => l(t('configuration'), 'admin/config/user-interface/fontyourface/fonts_com'))), 'error');
      fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

    }

  }
  else {

    backdrop_set_message(t('There was an error getting filter choices from Fonts.com. Check !configuration.', array('!configuration' => l(t('configuration'), 'admin/config/user-interface/fontyourface/fonts_com'))), 'error');
    fontyourface_log('Invalid backdrop_http_request response: @response', array('@response' => print_r($response, TRUE)));

  }

  return array();

}

/**
 * Publishes changes.
 */
function fonts_com_publish() {

  $path = '/rest/json/Publish/';

  $response = backdrop_http_request(
    FONTS_COM_API_BASE_URL . $path,
    array(
      'headers' => fonts_com_api_headers($path),
    )
  );

}

/**
 * Provides headers with api parameters.
 */
function fonts_com_api_headers($path) {

  $fonts_com_token = config_get('fontyourface.settings', 'fonts_com_token');

  if (empty($fonts_com_token)) {
    return array();
  }

  list($public_key, $private_key) = explode('--', $fonts_com_token);

  $encoded = base64_encode(hash_hmac('md5', $public_key . '|' . $path, $private_key, TRUE));
  $auth = urlencode($public_key . ':' . $encoded);

  return array('Authorization' => $auth, 'AppKey' => FONTS_COM_API_APP_KEY);

}

/**
 * Returns an array, regardless of input.
 */
function fonts_com_unknown_to_array($unknown) {

  if (is_array($unknown)) {
    return $unknown;
  }

  return array($unknown);

}
