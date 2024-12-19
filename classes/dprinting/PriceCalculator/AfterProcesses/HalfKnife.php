<?

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/BasicMaterials/Afterprocess.php');
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/AftPriceUtil.inc");

class HalfKnife extends Afterprocess {
    function __construct($product)
    {
        parent::__construct($product);
    }

    public function cost() {
        $util = new AftPriceUtil([
            "conn" => $this->conn,
            "dao"  => $this->dao
        ]);

        $param = [];
        $param["cate_sortcode"] = $this->sortcode;
        $param["after_name"]    = $this->depth;
        $param["amt"]           = $this->amt;
        $param["mpcode"]        = $this->depth1_val;
        $param["depth1"]        = $this->depth1;
        $param["depth2"]        = $this->depth2;
        $param["depth3"]        = $this->depth3;
        $param["size"]          = "";
        $ret = $util->getAfterPrice1($param);

        $result = $this->product->cost();
        $tmp_amt = $this->amt / 1000;
        $result[$this->after_name] = ($ret * $this->product->count * $tmp_amt);
        $result["add_after_price"] = $result["add_after_price"] + $result[$this->after_name];
        $result["sell_price"] = $result["sell_price"] + ($ret * $this->product->count);

        return $result;
    }
}
