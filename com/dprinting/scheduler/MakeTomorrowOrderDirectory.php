<?php

$datetime = new DateTime();
$datetime->modify('+1 day');

$mkdir = "/var/www/html/inc/attach/gp/order_file/";
$mkdir .= $datetime->format('Y') . "/" . $datetime->format('m') . "/" . $datetime->format('d');

$oldmask = umask(0);
@mkdir($mkdir, 0777);
umask($oldmask);
?>