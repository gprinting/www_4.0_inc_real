<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-10-27
 * Time: 오후 5:31
 */
define(INC_PATH, $_SERVER["INC"]);
include_once(INC_PATH . '/com/dprinting/StateDAO.inc');
include_once(INC_PATH . '/classes/dprinting/StateManager/abstracts/OrderByCountState.php');

abstract class AbstractOrderByCountState implements OrderByCountState {
    var $connectionPool;
    var $conn;
    var $dao;

    var $order_detail_file_num;
    var $origin_state;
    var $to_state;

    public function __construct() {
        $this->connectionPool = new ConnectionPool();
        $this->conn = $this->connectionPool->getPooledConnection();
        $this->dao = new StateDAO();

        /*
        $this->order_detail_file_num = $order_detail_file_num;

        $param = array();
        $param['order_detail_file_num'] = $order_detail_file_num;
        $this->origin_state = $this->dao->selectOrderDetailFileNumState($this->conn, $param);
        */
    }

    public function init($param) {
        $this->order_detail_file_num = $param['order_detail_file_num'];
    }

    public function ToNextState()
    {
        // 상태변경의 이력을 남기고
        // 낱건이 모두 동일한 상태값이 될 시 order_common, order_detail 또한 상태변경
        $param = array();
        if($this->to_state == "" || $this->to_state == NULL) {
            return false;
        }

        if($this->order_detail_file_num == "" || $this->order_detail_file_num == NULL) {
            return false;
        }

        $param['to_state'] = $this->to_state;
        $param['origin_state'] = $this->origin_state;
        $param['order_detail_file_num'] = $this->order_detail_file_num;

        $this->dao->updateCountFileState($this->conn, $param);
    }

    public function RollbackState()
    {
    }

    public function getState()
    {
        return $this->origin_state;
    }
}



?>