<?php

/**
 * Theme a timespan item.
 */
function theme_litecal_timeitem($item, $granularity) {
  $left = number_format($item->start / $granularity * 100, 2);
  $width = number_format($item->size / $granularity * 100, 2);

  $attr = array('style' => "left:{$left}%; width:{$width}%;");

  $attr['class'] = 'litecal-item';
  $attr['class'] .= $item->starts ? ' starts' : '';
  $attr['class'] .= $item->ends ? ' ends' : '';
  if (module_exists('crayon')) {
    $attr['class'] .= ' crayon-anchor crayon-'. crayon_generate_index($item->id);
    $popup = theme('crayon_popup', $item->data, $item->id);
  }
  else {
    $attr['class'] .= ' color';
    $popup = '';
  }

  if (!empty($item->url)) {
    return l("$popup<span class='litecal-data'>{$item->data}</span>", $item->url, array('attributes' => $attr, 'html' => TRUE));
  }
  else {
    $attr = drupal_attributes($attr);
    return "<div {$attr}>$popup<span class='litecal-data'>{$item->data}</span></div>";
  }
}

/**
 * Theme all timeslots for a given timespan.
 */
function theme_litecal_timeslots($timespan, $quickadd = array()) {
  switch ($timespan->unit) {
    case 'days':
      $format = 'j';
      break;
  }

  $slots = array();
  $date = drupal_clone($timespan->from);

  for ($i = 0; $i < $timespan->granularity; $i++) { 
    $slots[] = theme('litecal_timeslot', $timespan, $i, $date, $format, $quickadd);
    date_modify($date, "+1 {$timespan->unit}");
  }
  return $slots;
}

/**
 * Theme a single timeslot.
 */
function theme_litecal_timeslot($timespan, $start, $date, $format, $quickadd = array()) {
  $add = '';
  $attr = array('style' => '');
  $link_attr = array('class' => 'label');

  // Position
  if ($start < $timespan->granularity - 1) {
    $attr['style'] .= ' left:'. number_format($start / $timespan->granularity * 100, 2) .'%;';
  }
  // We position last items differently since slots often use borders and need tight alignment.
  else {
    $attr['style'] .= ' right:0%;';
  }

  // Width
  $attr['style'] .= ' width:'. number_format(1 / $timespan->granularity * 100, 2) .'%';

  // Classes
  $attr['class'] = 'litecal-slot rows-'. count($timespan->built);

  // Add class for today's slot.
  static $today;
  $today = !isset($today) ? date_format_date(date_now(), 'custom', 'Y-m-d') : $today;
  if ($today == date_format_date($date, 'custom', 'Y-m-d')) {
    $attr['class'] .= ' litecal-slot-today';
  }

  // If this timeslot is outside of the timespan's real time range,
  // add a class so it can be displayed accordingly.
  if (!litecal_date_between($date, $timespan->real_from, $timespan->real_to)) {
    $attr['class'] .= ' litecal-slot-gutter';
  }

  $attr = drupal_attributes($attr);

  // Quickadd
  if (!empty($quickadd['type'])) {
    $type = str_replace('_', '-', $quickadd['type']);
    $item = menu_get_item("node/add/{$type}");
    if ($item && $item['access']) {
      $options = array('query' => "edit[{$quickadd['field']}][0][value]=". date_convert($date, DATE_OBJECT, DATE_ISO) ."&destination=" . $_GET['q']);
      $link_attr['href'] = url("node/add/{$type}", $options);
      $add = "<span class='add'>". t('+ Add') ."</span>";
    }
  }
  $link_attr = drupal_attributes($link_attr);

  $formatted = date_format_date($date, 'custom', $format);

  return "<div {$attr}>
    <a $link_attr>{$add}<span class='num'>{$formatted}</span></a>
  </div>";
}

/**
 * Theme a header, like days of the week for a month.
 */
function theme_litecal_header($label, $start, $granularity) {
  $left = number_format($start / $granularity * 100, 2);
  $width = number_format(1 / $granularity * 100, 2);

  $attr = array('style' => "left:{$left}%; width:{$width}%;");
  $attr = drupal_attributes($attr);

  return "<div class='litecal-label' {$attr}>{$label}</div>";
}
