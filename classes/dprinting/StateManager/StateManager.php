<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-10-27
 * Time: 오후 7:54
 */

define(INC_PATH, $_SERVER["INC"]);
include_once(INC_PATH . "/com/nexmotion/common/util/ConnectionPool.inc");
include_once(INC_PATH . '/com/dprinting/StateDAO.inc');
include_once(INC_PATH . '/classes/dprinting/StateManager/StateFactory.php');

class StateManager {
    var $connectionPool;
    var $conn;
    var $dao;

    private $state;

    public function __construct($param) {
        $this->connectionPool = new ConnectionPool();
        $this->conn = $this->connectionPool->getPooledConnection();
        $this->dao = new StateDAO();

        $this->state = StateFactory::createState($param['origin_state']);
        $this->state->init($param);
    }

    public function ToNextState() {
        $this->state->ToNextState();

        /*
        $param = array();
        $param['order_detail_file_num'] = $this->state->order_detail_file_num;
        $state = $this->dao->selectCountFileState($this->conn, $param);
        $this->state = StateFactory::createState($state);
        $this->state->init($param);
        */
    }

    public function getState() {
        return $this->state->getState();
    }

    private function setState(OrderByCountState $state) {
        $this->state = $state;
    }
}