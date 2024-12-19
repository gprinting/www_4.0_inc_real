<?php
/**
 * Created by PhpStorm.
 * User: Hyeoonsik Cho
 * Date: 2017-10-27
 * Time: 오후 5:29
 */

interface OrderByCountState {
    public function ToNextState();
    public function RollbackState();
    public function getState();
}

?>