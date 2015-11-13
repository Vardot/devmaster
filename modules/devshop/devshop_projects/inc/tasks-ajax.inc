<?php

/**
 * Page callback that returns current tasks status.
 *
 * @param string $project_name
 */
function devshop_projects_tasks_status_json($project_nid = '') {

  $output = array();

  $sql = '
        SELECT
            project_nid,
            name,
            last_task,
            n.title as project,
            site,
            platform
        FROM {hosting_devshop_project_environment} h
        LEFT JOIN {node} n ON h.project_nid = n.nid

        ';

  if (!empty($project_nid)) {
    $sql .= ' WHERE project_nid = :project';
  }
  $results = db_query($sql, array(':project' => $project_nid));

  $task_types = hosting_available_tasks();

  // Get all environments and their status.
  foreach ($results as $result) {
    $result->last_task = (object) unserialize($result->last_task);

        // To test out constantly changing tasks, uncomment this line.
//        $result->last_task->status = rand(-1, 3);
    $result->last_task->status_name = devshop_task_status_class($result->last_task->status);

        // Skip verify and password tasks unless they are running
//        if ($result->last_task->status != HOSTING_TASK_PROCESSING && $result->last_task->status != HOSTING_TASK_QUEUED) {
//            if ($result->last_task->type == 'verify' || $result->last_task->type == 'login-reset') {
//                continue;
//            }
//        }

        // Override "ago" text.
        $result->last_task->ago = format_interval(REQUEST_TIME - $result->last_task->executed, 1) . ' ' . t('ago');
        if ($result->last_task->status == HOSTING_TASK_QUEUED) {
            $result->last_task->ago = t('Queued');
        }
        elseif ($result->last_task->status == HOSTING_TASK_PROCESSING) {
            $result->last_task->ago = t('Running...');
        }

        // Set task type
        $task_ref_type = $result->site == 0 ? 'platform' : 'site';
        $result->last_task->type_name = $task_types[$task_ref_type][$result->last_task->type]['title'];

        // Get Icon
        $result->last_task->icon = devshop_task_status_icon($result->last_task->status);
        $output[] = $result;
  }

  print json_encode($output);
  exit;
}