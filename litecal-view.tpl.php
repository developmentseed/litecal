<div class='litecal <?php print $class ?>'>

<div class='litecal-title clear-block'>
  <h3 class='litecal-title'><?php print $title ?></h3>
  <?php print theme('links', $links) ?>
</div>

<div class='litecal-header'>
<?php foreach ($header as $label): ?>
  <?php print $label; ?>
<?php endforeach; ?>
</div>

<?php foreach ($timespans as $num => $timespan): ?>
  <?php
  $class = '';
  $class .= ($num == 0) ? ' timespan-first' : '';
  $class .= ($num == count($timespans) - 1) ? ' timespan-last' : '';
  $class .= ' rows-'. count($timespan['rows']);
  ?>
  <div class='litecal-timespan <?php print $class ?>'>

  <?php foreach ($timespan['slots'] as $slot): ?>
    <?php print $slot ?>
  <?php endforeach; ?>

  <?php foreach ($timespan['rows'] as $row): ?>
    <div class='litecal-row clear-block'>
      <?php foreach ($row as $item): ?>
        <?php print $item; ?>
      <?php endforeach; ?>
    </div>
  <?php endforeach; ?>

  </div>
<?php endforeach; ?>

</div>