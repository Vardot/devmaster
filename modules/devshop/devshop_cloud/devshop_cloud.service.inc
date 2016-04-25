<?php

/**
 * The base service type class , registered with hook_hosting_service_type.
 */
class hostingService_provider extends hostingService {
  /**
   * the value stored in the service column of hosting_service table.
   */
  public $service = 'provider';

  /**
   * Options used to create the server.
   */
  public $provider_options = array();

  /**
   * Data returned back from provider about the server.
   */
  public $provider_data = array();

  /**
   * This server's unique identifier in the provider's system.
   */
  public $provider_server_identifier = '';

  /**
   * On server insert, we only have the provider_options.
   */
  function insert() {
    parent::insert();

    db_insert('hosting_devshop_cloud_provider')
      ->fields(array('nid', 'provider_data', 'provider_options', 'provider_server_identifier'), array(
        $this->server->nid,
        serialize($this->provider_data),
        serialize($this->provider_options),
        $this->provider_server_identifier
      ))
      ->execute();
  }

  function delete() {
    parent::delete();
    db_query('DELETE FROM {hosting_devshop_cloud_provider} WHERE nid = :nid', array(':nid' => $this->server->nid));
  }

  function load() {
    parent::load();
    $this->mergeData('SELECT * FROM {hosting_devshop_cloud_provider} WHERE nid = :nid', array(':nid' => $this->server->nid));
    $this->provider_options = unserialize($this->provider_options);
    $this->provider_data = unserialize($this->provider_data);
  }
}


/**
 * rackspace provider.
 */
class hostingService_provider_rackspace extends hostingService_provider {
  public $type = 'rackspace';
}