<?php
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/ConnectionPool.inc");
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/DAO/ProductDAO.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/CondimentDecorator.php');

class Plate extends CondimentDecorator {
    var $product;
    var $name;
    var $price;
    var $amt;
    var $count;

    var $dao;
    var $connectionPool;
    var $util;
    var $conn;

    function __construct($product) {
        $this->product = $product;
        $this->connectionPool = new ConnectionPool();
        $this->conn = $this->connectionPool->getPooledConnection();
        $this->dao = new ProductDAO();
    }

    function getDescription() {
        return $this->product->getDescription() . "판 : " . $this->name . "(" .$this->price . ")</br>";
    }

    function setPlate($cate_sortcode, $amt, $stan_mpcode, $paper_mpcode, $print_name, $print_purp) {
        $param = array();
        $param['cate_sortcode'] = $cate_sortcode;
        $param['amt'] = $amt;
        $param['stan_mpcode'] = $stan_mpcode;
        $param['paper_mpcode'] = $paper_mpcode;
        $param['tmpt'] = $print_name; // 단면 4도
        $param['purp']      = $print_purp; // 일반옵셋
        $param["table_name"]    = 'ply_price_gp';
        $param["print_mpcode"]  = $this->getPlateMpcode($param);

        if($param["print_mpcode"]) {
            $this->price = $this->dao->selectPrdtPlyPrice($this->conn, $param);
            $this->name = $print_name;
            $this->amt = $amt;
        } else {
            $this->price = 0;
            $this->name = '상품정보 없음';
        }
    }

    function getPlateMpcode($param) {
        $rs = $this->dao->selectCatePlateMpcode($this->conn, $param);
        return $rs->fields['mpcode'];
    }

    function getPrice($rs_price) {
        $this->price = $rs_price->fields['new_price'];
    }

    public function cost() {
        return $this->price + $this->product->cost();
    }

    public function getAfterList() {
        return $this->price + $this->product->cost();
    }
}
?>
