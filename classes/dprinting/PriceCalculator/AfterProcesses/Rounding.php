<?

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/BasicMaterials/Afterprocess.php');
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/AftPriceUtil.inc");

class Rounding extends Afterprocess {

    function __construct($product)
    {
        parent::__construct($product);

    }

    function makeOrderAfterHistoryInsertParam($fb) {
        $param = $this->getAfterList();
        $param["detail"] .= "(" . $fb["rounding_info"] . ")";
        $p = $this->getProduct();
        if($p->order_after_history_param == null)
            $p->order_after_history_param = array();
        array_push($p->order_after_history_param, $param);
        return $this->product->makeOrderAfterHistoryInsertParam($fb);
    }

    function getAfterList() {
        $param = array();
        $param['mpcode'] = $this->depth1_val;
        if($this->startsWith($this->sortcode, "002")) {
            $rs = $this->dao->selectCateAoAfterInfo($this->conn, $param);
        } else {
            $rs = $this->dao->selectCateAfterInfo($this->conn, $param);
        }
        while ($rs && !$rs->EOF) {
            $name = $rs->fields["after_name"];
            $depth1 = $rs->fields["depth1"];
            $depth2 = $rs->fields["depth2"];
            $depth3 = $rs->fields["depth3"];
            $depth1_dvs = $this->depth1_dvs;
            $mpcode = $rs->fields["mpcode"];
            $basic_yn = $rs->fields["basic_yn"];

            $ret["after_eng_name"] = $this->after_name;
            $ret["mpcode"]     = $mpcode;
            $ret["after_name"] = $name;
            $ret["depth1"]     = $depth1;
            $ret["depth2"]     = $depth2;
            $ret["depth3"]     = $depth3;
            $ret["basic_yn"]     = $basic_yn;

            $ret["detail"] = "";
            if($depth1 != "" && $depth1 != "-") {
                $ret["detail"] .= " " . $depth1;
            }

            if($depth2 != "" && $depth2 != "-") {
                $ret["detail"] .= " " . $depth2;
            }

            if($depth3 != "" && $depth3 != "-") {
                $ret["detail"] .= " " . $depth3;
            }

            if($depth1_dvs != "" && $depth1_dvs != "-") {
                $ret["detail"] .= " " . $depth1_dvs;
            }
            $ret["detail"] = trim($ret["detail"]);
            //$ret[$i]["price"]      = $info["price"];
            //$ret[$i]["detail"]     = $info["detail"];
            //$ret[$i]["info"]       = $info["info"];
            //$ret[$i++]["seq"]        = $i + 1;
            //$ret[$i++]["basic_yn"] = 'N';

            $rs->MoveNext();
        }
        //$result = $this->product->getAfterList();
        //array_push($result,$ret);
        return $ret;
    }
}
