<?

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/BasicMaterials/Afterprocess.php');
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/AftPriceUtil.inc");

class Cutting extends Afterprocess {
    function __construct($product)
    {
        parent::__construct($product);
    }

    public function cost() {
        if($this->sortcode == "005001001" || $this->sortcode == "005002001") {
            $util = new AftPriceUtil([
                "conn" => $this->conn,
                "dao" => $this->dao
            ]);

            $param = [];
            $param["cate_sortcode"] = $this->sortcode;
            $param["after_name"] = $this->depth;
            $param["amt"] = $this->depth1_cnt;
            $param["mpcode"] = $this->depth1_val;
            $param["depth1"] = $this->depth1;
            $param["depth2"] = $this->depth2;
            $param["depth3"] = $this->depth3;
            $param["size"] = $this->size_name;

            $basic_cut = 0;
            switch ($this->size_name) {
                case "A1":
                    $basic_cut = 0.25;
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
                    $basic_cut = 0.5;
                    $pos = 2000;
                    break;
                case "A4":
                case "8절(B4)":
                    $basic_cut = 1;
                    $pos = 4000;
                    break;
                case "A5":
                case "16절(B5)":
                    $basic_cut = 2;
                    $pos = 8000;
                    break;
                case "A6":
                case "32절(B6)":
                    $basic_cut = 4;
                    $pos = 16000;
                    break;
                case "64절(B7)":
                    $basic_cut = 8;
                    $pos = 32000;
                    break;
                default :
                    $basic_cut = 0;
                    $pos = 0;
            }

            switch ($this->size_name) {
                case "A1":
                case "2절(B2)":
                    $basic_cut = 0.25;
                    break;
                case "A2":
                case "4절(B3)":
                case "국장2절":
                case "국3절":
                    $basic_cut = 0.25;
                    break;
                case "A3":
                case "8절(B4)":
                    $basic_cut = 0.5;
                    break;
                case "A4":
                case "16절(B5)":
                    $basic_cut = 1;
                    break;
                case "A5":
                case "32절(B6)":
                    $basic_cut = 2;
                    break;
                case "A6":
                case "64절(B7)":
                    $basic_cut = 4;
                    break;
            }
            $cutting_cnt = 0;
            switch ($this->depth1_val) {
                case 7903:
                case 9342:
                    $cutting_cnt = 2;
                    break;
                case 7915:
                case 9354:
                    $cutting_cnt = 3;
                    break;
                case 7927:
                case 9366:
                    $cutting_cnt = 4;
                    break;
                case 7939:
                case 9378:
                    $cutting_cnt = 5;
                    break;
                case 7951:
                case 9390:
                    $cutting_cnt = 6;
                    break;
                case 8693:
                case 9402:
                    $cutting_cnt = 7;
                    break;
                case 8705:
                case 9414:
                    $cutting_cnt = 8;
                    break;
                default :
                    $cutting_cnt = 2;
            }

            $all_cut_cnt = $cutting_cnt * $basic_cut;
            $r = $this->amt / $pos;
            if($r < 1) $r = 1;

            $price = 0;
            if (strpos($this->size_name, "A") > -1
                || strpos($this->size_name, "국장3절") > -1
                || strpos($this->size_name, "국2절") > -1) {
                if ($all_cut_cnt <= 2) {
                    $price = 0;
                } else if ($all_cut_cnt <= 4) {
                    if ($r <= 5) {
                        $price = 3300;
                    } else if ($r <= 7) {
                        $price = 2200;
                    } else {
                        $price = 1650;
                    }
                } else if ($all_cut_cnt <= 7) {
                    if ($r <= 5) {
                        $price = 6600;
                    } else if ($r <= 7) {
                        $price = 4400;
                    } else {
                        $price = 3300;
                    }
                } else {
                    if ($r <= 5) {
                        $price = 16500;
                    } else if ($r <= 7) {
                        $price = 11000;
                    } else {
                        $price = 7700;
                    }
                }

            } else {
                if ($all_cut_cnt <= 2) {
                    $price = 0;
                } else if ($all_cut_cnt <= 4) {
                    if ($r <= 5) {
                        $price = 4400;
                    } else if ($r <= 7) {
                        $price = 3300;
                    } else {
                        $price = 2200;
                    }
                } else if ($all_cut_cnt <= 7) {
                    if ($r <= 5) {
                        $price = 8800;
                    } else if ($r <= 7) {
                        $price = 6600;
                    } else {
                        $price = 4400;
                    }
                } else {
                    if ($r <= 5) {
                        $price = 16500;
                    } else if ($r <= 7) {
                        $price = 11000;
                    } else {
                        $price = 7700;
                    }
                }
            }
            $price = $price * $r;
            $result = $this->product->cost();
            $result[$this->after_name] = ($price * $this->product->count);
            $result["sell_price"] = $result["sell_price"] + ($price * $this->product->count);

            return $result;
        } else {
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
            $ret = ceil($ret / 1.1 / 100) * 100 * 1.1;
            $result = $this->product->cost();
            $result[$this->after_name] = $ret * $this->product->count;
            $result["add_after_price"] = $result["add_after_price"] + $result[$this->after_name];
            $result["sell_price"] = $result["sell_price"] + ($ret * $this->product->count);
            $result["dvs"] = $this->dvs;

            return $result;
        }
    }
}
