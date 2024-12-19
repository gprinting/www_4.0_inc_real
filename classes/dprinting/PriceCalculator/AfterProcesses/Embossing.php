<?

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/BasicMaterials/Afterprocess.php');
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/AftPriceUtil.inc");

class Embossing extends Afterprocess {
    function __construct($product)
    {
        parent::__construct($product);
    }

    function cost() {
        $util = new AftPriceUtil([
            "conn" => $this->conn,
            "dao"  => $this->dao
        ]);
        $param = [];
        $param["cate_sortcode"] = $this->sortcode;
        $param["aft"]           = "embossing";
        $param["size_name"]           = $this->size_name;
        $param["amt"]           = $this->amt;
        $param["aft_1"]         = $this->depth1;
        $param["dvs_1"]         = $this->depth1_dvs;
        $param["aft_2"]         = $this->depth2;
        $param["dvs_2"]         = $this->depth2_dvs;
        $param["wid_1"]         = $this->depth1_wid;
        $param["vert_1"]        = $this->depth1_vert;
        $param["wid_2"]         = $this->depth2_wid;
        $param["vert_2"]        = $this->depth2_vert;

        $ret = $util->getAfterFoilPressPrice($param);
        $result = $this->product->cost();
        $result[$this->after_name] = ($ret["price"] * $this->product->count);
        $result["sell_price"] = $result["sell_price"] + ($ret["price"] * $this->product->count);
        return $result;
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

        $ret["detail"] = "";
        if($this->depth1 != "" && $this->depth1 != "-") {
            $ret["detail"] .= " " . $this->depth1;
        }

        if($this->depth2 != "" && $this->depth2 != "-") {
            $ret["detail"] .= " " . $this->depth2;
        }

        if($this->depth3 != "" && $this->depth3 != "-") {
            $ret["detail"] .= " " . $this->depth3;
        }

        if($this->depth1_dvs != "" && $this->depth1_dvs != "-") {
            $ret["detail"] .= " " . $this->depth1_dvs;
        }
        $ret["detail"] .= " " . $this->depth1_wid . "mm x " . $this->depth1_vert . "mm";
        $ret["detail"] = trim($ret["detail"]);

        return $ret;
    }
}
