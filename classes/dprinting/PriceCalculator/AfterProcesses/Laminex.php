<?

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/BasicMaterials/Afterprocess.php');
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/AftPriceUtil.inc");

class Laminex extends Afterprocess {
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
        $param["amt"]           = $this->depth1_cnt;
        $param["mpcode"]        = $this->depth1_val;
        $param["depth1"]        = $this->depth1;
        $param["depth2"]        = $this->depth2;
        $param["depth3"]        = $this->depth3;
        $param["size"]          = $this->size_name;

        /*
        switch ($this->size_name) {
            case "A1":
                $pos = 500;
                break;
            case "A2":
            case "2절(B2)":
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
        */
        $ret = $util->getAfterPrice1($param);

        $result = $this->product->cost();
        $result[$this->after_name] = ($ret * $this->product->count);
        $result["sell_price"] = $result["sell_price"] + ($ret * $this->product->count);

        return $result;
    }
}
