<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-10-27
 * Time: 오후 6:11
 * desc: 조판대기
 */

include_once(INC_PATH . "/classes/dprinting/StateManager/abstracts/AbstractOrderByCountState.php");

class PressWaitingState extends AbstractOrderByCountState
{
    public function ToNextState()
    {
        // 조판대기 -> 조판중
        $this->to_state = "2130";
        parent::ToNextState();
    }
}

?>