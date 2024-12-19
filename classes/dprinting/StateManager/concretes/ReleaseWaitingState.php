<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-11-08
 * Time: 오후 4:30
 */

include_once(INC_PATH . "/classes/dprinting/StateManager/abstracts/AbstractOrderByCountState.php");

class ReleaseWaitingState extends AbstractOrderByCountState
{
    var $dlvr_way;
    public function init($param) {
        // 필수정보
        $this->origin_state = $param['origin_state'];
        $this->to_state = "3320";

        $this->dlvr_way = $this->dao->selectDeliveryKind($this->conn, $param);
        if($this->dlvr_way == '06' || $this->dlvr_way == '07') {
            $this->to_state = "9120";
        }
        parent::init($param);
    }

    public function ToNextState()
    {
        parent::ToNextState();
    }
}