<?

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/BasicMaterials/Afterprocess.php');
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/AftPriceUtil.inc");

class Binding extends Afterprocess {
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
        $param["mpcode"]        = $this->depth1_val;
        $param["depth1"]        = $this->depth1;
        $param["depth2"]        = $this->depth2;
        $param["depth3"]        = $this->depth3;
        $param["size"]          = $this->size_name;

        if ($this->sortcode == "007001001"
            || $this->sortcode == "007001002"
            || $this->sortcode == "007001003"
            || $this->sortcode == "007002001"
            || $this->sortcode == "014002001") {
            $param["amt"] = $this->amt;

            if($param["mpcode"] == "8347") $param["mpcode"] = "822";
            if($param["mpcode"] == "8348") $param["mpcode"] = "823";
            if($param["mpcode"] == "8349") $param["mpcode"] = "824";
            if($param["mpcode"] == "8350") $param["mpcode"] = "825";
            if($param["mpcode"] == "8351") $param["mpcode"] = "826";
            if($param["mpcode"] == "8352") $param["mpcode"] = "827";
            if($param["mpcode"] == "8353") $param["mpcode"] = "828";
            if($param["mpcode"] == "8354") $param["mpcode"] = "829";
            if($param["mpcode"] == "8355") $param["mpcode"] = "830";
            if($param["mpcode"] == "8356") $param["mpcode"] = "831";
        } else {
            switch ($this->size_name) {
                case "A1":
                    $pos = 500;
                    break;
                case "국3절":
                    $pos = 750;
                    break;
                case "A2":
                case "2절(B2)":
                case "국장2절":
                    $pos = 1000;
                    break;
                case "A3":
                case "4절(B3)":
                    $pos = 2000;
                    break;
                case "A4":
                case "8절(B4)":
                    $pos = 4000;
                    break;
                case "A5":
                case "16절(B5)":
                    $pos = 8000;
                    break;
                case "A6":
                case "32절(B6)":
                    $pos = 16000;
                    break;
                case "64절(B7)":
                    $pos = 32000;
                    break;
                default :
                    $pos = 0;
            }

            $param["amt"] = $this->amt / $pos;
        }

        $ret = $util->getAfterPrice1($param);
        $ret = ceil($ret / 100) * 100;
        $result = $this->product->cost();
        $result[$this->after_name] = $ret * $this->product->count;
        $result["add_after_price"] = $result["add_after_price"] + $result[$this->after_name];
        $result["sell_price"] = $result["sell_price"] + ($ret * $this->product->count);
        $result["dvs"] = $this->dvs;

        return $result;
    }
}
