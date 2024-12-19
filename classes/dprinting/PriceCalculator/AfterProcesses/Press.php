<?

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/BasicMaterials/Afterprocess.php');
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/AftPriceUtil.inc");

class Press extends Afterprocess {

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
        $param["aft"]           = "press";
        $param["amt"]           = $this->amt;
        $param["aft_1"]         = $this->depth1;
        $param["dvs_1"]         = $this->depth1_dvs;
        $param["aft_2"]         = $this->depth2;
        $param["dvs_2"]         = $this->depth2_dvs;
        $param["wid_1"]         = $this->depth1_wid;
        $param["vert_1"]        = $this->depth1_vert;
        $param["wid_2"]         = $this->depth2_wid;
        $param["vert_2"]        = $this->depth2_vert;

        $product = $this->product->getProduct();
        $size_area = $product->size_w * $product->size_h;
        if($this->sortcode == "001001001") {
            $size_area *= 2;
        }
        $size_price = $size_area * 0.8;
        $pan_area = $this->depth1_wid * $this->depth1_vert;
        $amt_price = $this->amt * 35;

        // 사이즈에 따라 가격을 나누어야함
        if($this->amt < 500) $amt_price * 1.2;

        if($pan_area < 100) $pan_area = 100;
        $pan_area *= 1.5;

        $ret["price"] = ($size_price + $pan_area) + $amt_price;
        if(($this->sortcode == "003001001" || $this->sortcode == "003002001")
            && (($product->size_w == 90 && $product->size_h == 50)
                ||($product->size_w == 50 && $product->size_h == 90)
                ||($product->size_w == 86 && $product->size_h == 52)
                ||($product->size_w == 52 && $product->size_h == 86))) {
            $ret["price"] = $ret["price"] * 0.65;
        }

        /*
        echo "사이즈 가격 : " . $size_price;
        echo "동판 가격 : " . $pan_area;
        echo "매수 가격 : " . $amt_price;
        echo "합계 가격 : " . ($size_price + $pan_area + $amt_price);
*/


        $ret["price"] = ceil($ret["price"] / 1.1 / 100) * 100 * 1.1;
        $result = $this->product->cost();
        $result[$this->after_name] = ($ret["price"] * $product->count);
        $result["add_after_price"] = $result["add_after_price"] + $result[$this->after_name];
        $result["sell_price"] = $result["sell_price"] + ($ret["price"] * $product->count);
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
        $ret["detail"]     = $this->depth1_wid . "mm x " . $this->depth1_vert . "mm";

        return $ret;
    }
}
