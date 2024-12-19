<?

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/BasicMaterials/Afterprocess.php');
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/AftPriceUtil.inc");

class Bonding extends Afterprocess {
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

        if($pos == 0) {
            //echo $this->depth1_cnt. "11";
            if($this->sortcode == "006001001"
                || $this->sortcode == "006001002"
                || $this->sortcode == "006001003"
                || $this->sortcode == "006001004"
                || $this->sortcode == "006001005"
                || $this->sortcode == "006001006"
                || $this->sortcode == "006001007") {
                if ($this->sortcode == "006001001") {	//	대봉투

                    if ($this->amt >= 1000) {
                        $ret = 17		* ($this->amt * $this->count);	//	천장단위로 동일 금액 증가
                    } else {
                        $ret = 18		* ($this->amt * $this->count);	//	1000장 미만
                    }
                } else if ($this->sortcode == "006001002") {	//	중봉투

                    if ($this->amt >= 1000) {
                        $ret = 17		* ($this->amt * $this->count);	//	천장단위로 동일 금액 증가
                    } else {
                        $ret = 18		* ($this->amt * $this->count);	//	1000장 미만
                    }
                } else if ($this->sortcode == "006001003"
                    || $this->sortcode == "006001004"
                    || $this->sortcode == "006001005"
                    || $this->sortcode == "006001006"
                    || $this->sortcode == "006001007") {	//	소봉투

                    if($this->amt >= 1000) {
                        $ret = 14		* ($this->amt * $this->count);	//	천장단위로 동일 금액 증가
                    } else {
                        $ret = 16		* ($this->amt * $this->count);	//	1000장 미만
                    }
                }
                $ret = ceil($ret / 100) * 100;
            } else {
                $ret = $util->getAfterPrice1($param);
                $ret = ceil($ret / 100) * 100;
            }
        } else {
            $r_count = $this->amt / $pos;

            $depth = $util->getAfterDepth($param);

            $depth1 = $depth["depth1"];
            $depth2 = $depth["depth2"];

            $base_price = -1;
            $add_price = -1;
            $minimum_price = -1;
            if($this->size_name == "A1") {
                if($depth1 == "1면접착" && $depth2 == "수동") {
                    $base_price = 30000;
                    $add_price = 10000;
                    $minimum_price = 40000;
                }

                if($depth1 == "1면접착" && $depth2 == "자동") {
                    $base_price = 12500;
                    $add_price = 20000;
                    $minimum_price = 60000;
                }

                if($depth1 == "2면접착" && $depth2 == "수동") {
                    $base_price = 50000;
                    $add_price = 10000;
                    $minimum_price = 60000;
                }

                if($depth1 == "3면접착" && $depth2 == "자동") {
                    $base_price = 12500;
                    $add_price = 30000;
                    $minimum_price = 70000;
                }
            }

            if($this->size_name == "A2") {
                if($depth1 == "1면접착" && $depth2 == "수동") {
                    $base_price = 40000;
                    $add_price = 10000;
                    $minimum_price = 50000;
                }

                if($depth1 == "1면접착" && $depth2 == "자동") {
                    $base_price = 16000;
                    $add_price = 20000;
                    $minimum_price = 60000;
                }

                if($depth1 == "2면접착" && $depth2 == "수동") {
                    $base_price = 75000;
                    $add_price = 10000;
                    $minimum_price = 85000;
                }

                if($depth1 == "3면접착" && $depth2 == "자동") {
                    $base_price = 16000;
                    $add_price = 30000;
                    $minimum_price = 70000;
                }
            }

            if($this->size_name == "A3") {
                if($depth1 == "1면접착" && $depth2 == "수동") {
                    $base_price = 76000;
                    $add_price = 10000;
                    $minimum_price = 86000;
                }

                if($depth1 == "1면접착" && $depth2 == "자동") {
                    $base_price = 26000;
                    $add_price = 20000;
                    $minimum_price = 60000;
                }

                if($depth1 == "2면접착" && $depth2 == "수동") {
                    $base_price = 140000;
                    $add_price = 10000;
                    $minimum_price = 150000;
                }

                if($depth1 == "3면접착" && $depth2 == "자동") {
                    $base_price = 26000;
                    $add_price = 30000;
                    $minimum_price = 70000;
                }
            }

            if($this->size_name == "A4") {
                if($depth1 == "1면접착" && $depth2 == "수동") {
                    $base_price = 128000;
                    $add_price = 10000;
                    $minimum_price = 74000;
                }

                if($depth1 == "1면접착" && $depth2 == "자동") {
                    $base_price = 40000;
                    $add_price = 20000;
                    $minimum_price = 60000;
                }

                if($depth1 == "2면접착" && $depth2 == "수동") {
                    $base_price = 220000;
                    $add_price = 10000;
                    $minimum_price = 120000;
                }

                if($depth1 == "3면접착" && $depth2 == "자동") {
                    $base_price = 40000;
                    $add_price = 30000;
                    $minimum_price = 60000;
                }
            }

            if($this->size_name == "A5") {
                if($depth1 == "1면접착" && $depth2 == "수동") {
                    $base_price = 256000;
                    $add_price = 10000;
                }

                if($depth1 == "1면접착" && $depth2 == "자동") {
                    $base_price = 80000;
                    $add_price = 20000;
                }

                if($depth1 == "2면접착" && $depth2 == "수동") {
                    $base_price = 440000;
                    $add_price = 10000;
                }

                if($depth1 == "3면접착" && $depth2 == "자동") {
                    $base_price = 80000;
                    $add_price = 30000;
                }
            }

            if($this->size_name == "A6") {
                if($depth1 == "1면접착" && $depth2 == "수동") {
                    $base_price = 512000;
                    $add_price = 10000;
                }

                if($depth1 == "1면접착" && $depth2 == "자동") {
                    $base_price = 160000;
                    $add_price = 20000;
                }

                if($depth1 == "2면접착" && $depth2 == "수동") {
                    $base_price = 880000;
                    $add_price = 10000;
                }

                if($depth1 == "3면접착" && $depth2 == "자동") {
                    $base_price = 160000;
                    $add_price = 30000;
                }
            }

            if($this->size_name == "2절(B2)") {
                if($depth1 == "1면접착" && $depth2 == "수동") {
                    $base_price = 50000;
                    $add_price = 10000;
                    $minimum_price = 60000;
                }

                if($depth1 == "1면접착" && $depth2 == "자동") {
                    $base_price = 22000;
                    $add_price = 20000;
                    $minimum_price = 60000;
                }

                if($depth1 == "2면접착" && $depth2 == "수동") {
                    $base_price = 85000;
                    $add_price = 10000;
                    $minimum_price = 95000;
                }

                if($depth1 == "3면접착" && $depth2 == "자동") {
                    $base_price = 22000;
                    $add_price = 30000;
                    $minimum_price = 70000;
                }
            }

            if($this->size_name == "4절(B3)") {
                if($depth1 == "1면접착" && $depth2 == "수동") {
                    $base_price = 80000;
                    $add_price = 10000;
                    $minimum_price = 90000;
                }

                if($depth1 == "1면접착" && $depth2 == "자동") {
                    $base_price = 32000;
                    $add_price = 20000;
                    $minimum_price = 60000;
                }

                if($depth1 == "2면접착" && $depth2 == "수동") {
                    $base_price = 150000;
                    $add_price = 10000;
                    $minimum_price = 160000;
                }

                if($depth1 == "3면접착" && $depth2 == "자동") {
                    $base_price = 32000;
                    $add_price = 30000;
                    $minimum_price = 70000;
                }
            }

            if($this->size_name == "8절(B4)") {
                if($depth1 == "1면접착" && $depth2 == "수동") {
                    $base_price = 140000;
                    $add_price = 10000;
                    $minimum_price = 90000;
                }

                if($depth1 == "1면접착" && $depth2 == "자동") {
                    $base_price = 40000;
                    $add_price = 20000;
                    $minimum_price = 60000;
                }

                if($depth1 == "2면접착" && $depth2 == "수동") {
                    $base_price = 240000;
                    $add_price = 10000;
                    $minimum_price = 130000;
                }

                if($depth1 == "3면접착" && $depth2 == "자동") {
                    $base_price = 40000;
                    $add_price = 30000;
                    $minimum_price = 70000;
                }
            }

            if($this->size_name == "16절(B5)") {
                if($depth1 == "1면접착" && $depth2 == "수동") {
                    $base_price = 256000;
                    $add_price = 10000;
                }

                if($depth1 == "1면접착" && $depth2 == "자동") {
                    $base_price = 80000;
                    $add_price = 20000;
                }

                if($depth1 == "2면접착" && $depth2 == "수동") {
                    $base_price = 440000;
                    $add_price = 10000;
                }

                if($depth1 == "3면접착" && $depth2 == "자동") {
                    $base_price = 80000;
                    $add_price = 30000;
                }
            }

            if($this->size_name == "32절(B6)") {
                if($depth1 == "1면접착" && $depth2 == "수동") {
                    $base_price = 512000;
                    $add_price = 10000;
                }

                if($depth1 == "1면접착" && $depth2 == "자동") {
                    $base_price = 160000;
                    $add_price = 20000;
                }

                if($depth1 == "2면접착" && $depth2 == "수동") {
                    $base_price = 880000;
                    $add_price = 10000;
                }

                if($depth1 == "3면접착" && $depth2 == "자동") {
                    $base_price = 160000;
                    $add_price = 30000;
                }
            }

            if($this->size_name == "64절(B7)") {
                if($depth1 == "1면접착" && $depth2 == "수동") {
                    $base_price = 512000;
                    $add_price = 10000;
                }

                if($depth1 == "1면접착" && $depth2 == "자동") {
                    $base_price = 320000;
                    $add_price = 20000;
                }

                if($depth1 == "2면접착" && $depth2 == "수동") {
                    $base_price = 880000;
                    $add_price = 10000;
                }

                if($depth1 == "3면접착" && $depth2 == "자동") {
                    $base_price = 320000;
                    $add_price = 30000;
                }
            }

            $ret = $r_count * $base_price + $add_price;
            if($minimum_price != -1 && $ret < $minimum_price)
                $ret = $minimum_price;


        }
        $ret *= 1.1;
        $result = $this->product->cost();
       // $result[$this->after_name] = $ret * $this->product->count;
        $result[$this->after_name] = $ret ;
        $result["add_after_price"] = $result["add_after_price"] + $result[$this->after_name];
        $result["sell_price"] = $result["sell_price"] + ($ret * $this->product->count);
        $result["dvs"] = $this->dvs;
        return $result;
    }
}
