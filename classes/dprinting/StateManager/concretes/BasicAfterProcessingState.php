<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-11-08
 * Time: 오후 3:07
 */

include_once(INC_PATH . "/classes/dprinting/StateManager/abstracts/AbstractOrderByCountState.php");

class BasicAfterProcessingState extends AbstractOrderByCountState
{
    var $sheet_typset_seqno;
    var $kind;
    public function init($param) {
        // 필수정보
        $this->sheet_typset_seqno = $param['sheet_typset_seqno'];
        $this->origin_state = $param['origin_state'];
        $this->kind = $param['kind'];
        //parent::init($param);
        if($param['kind'] == "hold")
            $this->to_state = "2460";
        else if($param['kind'] == "progress")
            $this->to_state = "3120";
    }

    public function ToNextState()
    {
        $param['sheet_typset_seqno'] = $this->sheet_typset_seqno;
        $param['to_state'] = $this->to_state;
        $param['origin_state'] = $this->origin_state;
        $this->dao->updateTypsetState($this->conn, $param);

        $order_detail_file_nums = $this->dao->selectOrderDetailCountFileSeqno($this->conn, $param);
        foreach($order_detail_file_nums as $order_detail_file_num) {
            $param['order_detail_file_num'] = $order_detail_file_num;
            if($this->dao->isIncludedTypset($this->conn, $param)) {
                if($this->kind == "progress") {
                    if($this->dao->DoesHaveAfterProcess($this->conn, $param)) {
                        $this->to_state = "2520";
                    } else {
                        $this->to_state = "3120";
                    }
                }

                $this->order_detail_file_num = $order_detail_file_num;
                parent::ToNextState();
            }
        }
    }

    public function RollbackState()
    {
        // TODO: Change the autogenerated stub
        parent::RollbackState();
    }
}