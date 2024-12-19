<?

include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/PrintoutInterface.php');
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/BasicMaterials/Afterprocess.php');
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/front/AftPriceUtil.inc");

class Impression extends Afterprocess {

    function __construct($product)
    {
        $this->size_w = $product->size_w;
        $this->size_h = $product->size_h;
        parent::__construct($product);

    }

    public function cost() {
        if ($this->sortcode == "011001001"
            || $this->sortcode == "011008001") {
            $util = new AftPriceUtil([
                "conn" => $this->conn,
                "dao"  => $this->dao
            ]);
            $p = $this->getProduct();
            $long_side = $p->size_w;
            if($long_side < $p->size_h)
                $long_side = $p->size_h;

            $rate = 1;
            if($long_side > 400)
                $rate = 3;
            else if($long_side > 350)
                $rate = 2.5;
            else if($long_side > 300)
                $rate = 1.8;
            else if($long_side > 200)
                $rate = 1.4;
            else if($long_side > 150)
                $rate = 1.2;

            $param = [];
            $param["cate_sortcode"] = $this->sortcode;
            $param["after_name"]    = $this->depth;
            $param["mpcode"]        = $this->depth1_val;
            $param["depth1"]        = $this->depth1;
            $param["depth2"]        = $this->depth2;
            $param["depth3"]        = $this->depth3;
            $param["size"]          = $this->size_name;
            $param["amt"]           = $this->amt;

            $ret = $util->getAfterPrice1($param) * $rate;

            $ret = ceil($ret / 100) * 100;
            $result = $this->product->cost();
            $result[$this->after_name] = $ret * $this->product->count;
            $result["add_after_price"] = $result["add_after_price"] + $result[$this->after_name];
            $result["sell_price"] = $result["sell_price"] + ($ret * $this->product->count);
            $result["dvs"] = $this->dvs;

            return $result;

        } else {
            return parent::cost();
        }
    }
}
