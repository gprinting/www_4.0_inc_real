<?

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/BasicMaterials/Afterprocess.php');
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/AftPriceUtil.inc");

class Ring extends Afterprocess {
    function __construct($product)
    {
        parent::__construct($product);
    }

    public function cost() {
        $price = intval($this->amt) * 1100;
        //20240604 건수오류 수정 ksh
        if($this->count > 1){
            $price = (intval($this->amt) * intval($this->count)) * 1100;
        }

        $result = $this->product->cost();
        $result[$this->after_name] = $price;
        $result["add_after_price"] = $result["add_after_price"] + $result[$this->after_name];
        $result["sell_price"] = $result["sell_price"] + $price;

        return $result;
    }

    function getAfterList() {
        $param = array();
        $param['mpcode'] = $this->depth1_val;
        $rs = $this->dao->selectCateAoAfterInfo($this->conn, $param);
        while ($rs && !$rs->EOF) {
            $name = $rs->fields["after_name"];
            $depth1 = $rs->fields["depth1"];
            $depth2 = $rs->fields["depth2"];
            $depth3 = $rs->fields["depth3"];
            $mpcode = $rs->fields["mpcode"];
            $basic_yn = $rs->fields["basic_yn"];

            $ret["after_eng_name"] = $this->after_name;
            $ret["mpcode"]     = $mpcode;
            $ret["after_name"] = $name;
            $ret["depth1"]     = $depth1;
            $ret["depth2"]     = $depth2;
            $ret["depth3"]     = $depth3;
            $ret["basic_yn"]     = $basic_yn;
            $ret["detail"]     = $this->depth1_dvs;
            //$ret[$i]["price"]      = $info["price"];
            //$ret[$i]["detail"]     = $info["detail"];
            //$ret[$i]["info"]       = $info["info"];
            //$ret[$i++]["seq"]        = $i + 1;
            //$ret[$i++]["basic_yn"] = 'N';

            $rs->MoveNext();
        }
        return $ret;
    }
}
