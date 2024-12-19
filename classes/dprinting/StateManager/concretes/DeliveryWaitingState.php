<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-10-28
 * Time: 오후 3:59
 * desc: 배송대기
 */

include_once(INC_PATH . "/classes/dprinting/StateManager/abstracts/AbstractOrderByCountState.php");

class DeliveryWaitingState extends AbstractOrderByCountState
{
    var $dlvr_way;
    var $bun_dlvr_order_num;
    var $bun_group;
    public function init($param) {
        // 필수정보
        $this->origin_state = $param['origin_state'];
        $this->to_state = "3330";
        $this->dlvr_way = $param['kind'];

        $rs = $this->dao->selectBunInfo($this->conn, $param);
        while($rs && !$rs->EOF) {
            $this->bun_dlvr_order_num = $rs->fields['bun_dlvr_order_num'];
            $this->bun_group = $rs->fields['bun_group'];
            $rs->MoveNext();
        }

        parent::init($param);
    }

    public function ToNextState()
    {
        $param = array();
        $param['bun_dlvr_order_num'] = $this->bun_dlvr_order_num;
        $param['bun_group'] = $this->bun_group;

        $rs = $this->dao->selectOrderDetailFileNumsFromBunInfo($this->conn, $param);
        while($rs && !$rs->EOF) {
            $this->order_detail_file_num = $rs->fields['order_detail_file_num'];
            parent::ToNextState();
            $rs->MoveNext();
        }
    }
}