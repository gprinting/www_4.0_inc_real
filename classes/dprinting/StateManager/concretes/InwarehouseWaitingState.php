<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-10-28
 * Time: 오후 3:52
 */

include_once(INC_PATH . "/classes/dprinting/StateManager/abstracts/AbstractOrderByCountState.php");

class InwarehouseWatingState extends AbstractOrderByCountState
{
    public function init($param) {
        // 필수정보
        $this->origin_state = $param['origin_state'];
        parent::init($param);
    }

    public function ToNextState()
    {
        $this->to_state = "3220";
        parent::ToNextState();
    }
}