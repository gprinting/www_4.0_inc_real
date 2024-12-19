<?

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/BasicMaterials/Afterprocess.php');
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/AftPriceUtil.inc");

class Thomson extends Afterprocess {
    function __construct($product)
    {
        parent::__construct($product);
    }

    function getAfterList() {
        $ret["mpcode"]     = 0;
        $ret["after_eng_name"] = $this->after_name;
        $ret["after_name"] = $this->depth;
        //echo $this->depth;
        $ret["depth1"]     = $this->depth1;
        //echo $this->depth1;
        $ret["depth2"]     = "-";
        $ret["depth3"]     = "-";
        $ret["basic_yn"]     = "N";
        $ret["detail"]     = $this->depth1_wid . "mm x " . $this->depth1_vert . "mm";

        return $ret;
    }

    function cost() {
        $util = new AftPriceUtil([
            "conn" => $this->conn,
            "dao"  => $this->dao
        ]);
        $param = [];
        $param["cate_sortcode"] = $this->sortcode;
        $param["stan_mpcode"] = $this->product->stan_mpcode;
        $param["aft"]           = "thomson";
        $param["amt"]           = $this->amt;
        $param["aft_1"]         = $this->depth1;
        $param["dvs_1"]         = $this->depth1_dvs;
        $param["cnt_1"]         = $this->depth1_cnt;
        $param["aft_2"]         = $this->depth2;
        $param["dvs_2"]         = $this->depth2_dvs;
        $param["wid_1"]         = $this->depth1_wid;
        $param["vert_1"]        = $this->depth1_vert;
        $param["wid_2"]         = $this->depth2_wid;
        $param["vert_2"]        = $this->depth2_vert;

        //$pan_area = $this->depth1_wid * $this->depth1_vert * 0.3;
        $pan_area = $this->product->size_w * $this->product->size_h * 0.2;

        $param["wid_1"]         = $this->depth1_wid;
        $param["vert_1"]        = $this->depth1_vert;
        $cnt_1 = $param["cnt_1"];

        $size_price = ($this->product->size_w * $this->product->size_h) * 0.2;
        //$size_price = 0;
        $amt_price = $this->amt * 16;

        //if($this->amt <= 4000) $amt_price * 1.5;
        if($pan_area < 100) $pan_area = 100;
        //$pan_area *= 0.5;

        if($cnt_1 == 1) {
            $pan_area *= 1;
        } else if($cnt_1 == 2) {
            $pan_area *= 1.3;
        } else if($cnt_1 == 3) {
            $pan_area *= 1.6;
        } else if($cnt_1 == 4) {
            $pan_area *= 1.9;
        } else if($cnt_1 == 5) {
            $pan_area *= 2.1;
        } else if($cnt_1 == 6) {
            $pan_area *= 2.4;
        }

        $ret["price"] = ($size_price + $pan_area) + $amt_price;

        if($ret["price"] < 55000) $ret["price"] = 55000;
        $ret["price"] = ceil($ret["price"] / 100) * 100;
        $result = $this->product->cost();
        $result[$this->after_name] = ($ret["price"] * $this->product->count);
        $result["add_after_price"] = $result["add_after_price"] + $result[$this->after_name];
        $result["sell_price"] = $result["sell_price"] + ($ret["price"] * $this->product->count);
        return $result;
    }
}
