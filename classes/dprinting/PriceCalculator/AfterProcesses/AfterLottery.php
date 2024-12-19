<?

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/BasicMaterials/Afterprocess.php');
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/AftPriceUtil.inc");

class AfterLottery extends Afterprocess {
    function __construct($product)
    {
        parent::__construct($product);
    }

    function getAfterList() {
        $ret["mpcode"]     = 0;
        $ret["after_eng_name"] = $this->after_name;
        $ret["after_name"] = "복권가공";
        $ret["depth1"]     = "-";
        $ret["depth2"]     = "-";
        $ret["depth3"]     = "-";
        $ret["basic_yn"]     = "N";
        $ret["detail"]     = $this->depth1_val;

        return $ret;
    }

    function cost() {
        $result = $this->product->cost();
        $result[$this->after_name] = 0;
        $result["add_after_price"] = $result["add_after_price"] + $result[$this->after_name];
        $result["sell_price"] = $result["sell_price"] + 0;
        return $result;
    }
}
