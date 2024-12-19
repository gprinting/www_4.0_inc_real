<?
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/ConnectionPool.inc");
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/CondimentDecorator.php');

class Discount extends CondimentDecorator {
    var $product;
    var $name;
    var $price;
    var $sortcode;
    var $amt;
    var $grade;

    var $dao;
    var $connectionPool;
    var $util;
    var $conn;

    function __construct($product) {
        $this->connectionPool = new ConnectionPool();
        $this->conn = $this->connectionPool->getPooledConnection();
        $this->dao = new ProductCommonDAO();
        $this->product = $product;
    }

    function getDescription() {
        return $this->product->getDescription() . "후공정 : " . $this->name . "(" .$this->price . ")</br>";
    }

    function getPrice($rs_price) {
        return $rs_price->fields['sell_price'];
    }

    public function cost() {
        $param = array();
        $param["sortcode"] = $this->product->sortcode;
        $param["sell_site"] = $_SERVER["SELL_SITE"];
        $rs = $this->dao->selectPriceRate($this->conn, $param);

        $rate = $rs["rate"];
        $basic_rate = $rs["basic_rate"];

        if($rate == null) {
            $rate = $basic_rate;
        }
        //40
        $this->getProduct()->discount_rate = ($rate + 100) / 100;
        //$this->product->discount_rate = ($rate + 100) / 100;
        $result = $this->product->cost();

        $result["grade_aplc"] = 0;
        $result["grade_rate"] = 0;
        //var_dump($result);
        //if($this->grade == "1") {
        //    $result["grade_rate"] = 7;
        //    $result["grade_aplc"] = $result["sell_price"] * 0.07;
        //    $result["sell_price"] = $result["sell_price"];
        //}
        return $result;
    }

    function getProduct() {
        return $this->product->getProduct();
    }

    function makeOrderCommonInsertParam($fb, $session) {
        return $this->product->makeOrderCommonInsertParam($fb, $session);
    }

    function makeOrderDetailInsertParam($fb, $session) {
        return $this->product->makeOrderDetailInsertParam($fb, $session);
    }

    function makeOrderDlvrInsertParam($fb) {
        return $this->product->makeOrderDlvrInsertParam($fb);
    }

    function makeOrderAfterHistoryInsertParam($fb) {
        return $this->product->makeOrderAfterHistoryInsertParam($fb);
    }

    function makeOrderOptionHistoryInsertParam($fb) {
        return $this->product->makeOrderOptionHistoryInsertParam($fb);
    }

    function makeOrderFileInsertParam($fb) {
        return $this->product->makeOrderFileInsertParam($fb);
    }

    function getJarisu() {
        return $this->product->jarisu;
    }

    public function getAfterList() {
        return $this->product->getAfterList();
    }

    public function getOptionList() {
        return $this->product->getOptionList();
    }
}


?>
