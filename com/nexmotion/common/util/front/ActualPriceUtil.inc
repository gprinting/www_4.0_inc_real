<?php
class ActualPriceUtil {
    // 현수막 헤베당 단가
    private const NORMAL_PLACARD_UNITPRICE = 3900; // ~29
    private const LARGE_PLACARD_UNITPRICE1 = 4900; // 30~49
    private const LARGE_PLACARD_UNITPRICE2 = 4500; // 50~99
    private const LARGE_PLACARD_UNITPRICE3 = 3900; // 100~

    private $hebe;
    private $hebe_h;
    private $hebe_v;

    public function calcPrice($w, $h): int {
        $hebe = $this->calcHebe($w, $h);
        $unitprice = 0;

        if ($hebe < 30) {
            $unitprice = self::NORMAL_PLACARD_UNITPRICE;
        } else if (30 <= $hebe && $hebe < 50) {
            $unitprice = self::LARGE_PLACARD_UNITPRICE1;
        } else if (50 <= $hebe && $hebe < 100) {
            $unitprice = self::LARGE_PLACARD_UNITPRICE2;
        } else if (100 <= $hebe) {
            $unitprice = self::LARGE_PLACARD_UNITPRICE3;
        }

        $price = $hebe * $unitprice * 1.1;

        if ($price < 3900) {
            $price = self::NORMAL_PLACARD_UNITPRICE * 1.1;
        }

        return $price;
    }

    /**
     * @brief 헤베 계산
     *
     * @param $w = 너비 mm
     * @param $h = 높이 mm
     *
     * @return 헤베값
     */
    public function calcHebe($w, $h): int {
        $horizon  = doubleval($w) / 1000.0; 
        $vertical = doubleval($h) / 1000.0; 

        $this->hebe   = $horizon * $vertical;
        $this->hebe_h = $horizon;
        $this->hebe_v = $vertical;

        return $horizon * $vertical;
    }

    public function getHebe() {
        return $this->hebe;
    }

    public function getHebeH() {
        return $this->hebe_h;
    }

    public function getHebeV() {
        return $this->hebe_v;
    }

    /**
     * @brief 후공정 가격 계산
     *
     * @param $param = 가격 계산용 파라미터
     * @detail unitprice = 단가
     * pos_arr = 미싱류 위치값 true / fales 배열
     * cnt_arr = 고리류 위치별 개수
     * amt = 단가에 곱할 수량
     * wid = 헤베 구할 너비값
     * vert = 헤베 구할 높이값
     *
     * @return 가격
     */
    public function calcAfterPrice($param) {
        $price = 0;

        $unitprice = intval($param["unitprice"]);
        $amt  = intval($param["amt"]);

        $wid  = $param["wid"];
        $vert = $param["vert"];

        // 미싱류 위치
        $t_pos = intval($param["pos_arr"]["t"]);
        $b_pos = intval($param["pos_arr"]["b"]);
        $l_pos = intval($param["pos_arr"]["l"]);
        $r_pos = intval($param["pos_arr"]["r"]);
        // 고리류 위치별 개수
        $t_cnt = intval($param["cnt_arr"]["t"]);
        $b_cnt = intval($param["cnt_arr"]["b"]);
        $l_cnt = intval($param["cnt_arr"]["l"]);
        $r_cnt = intval($param["cnt_arr"]["r"]);

        if ($t_pos > 0 || $b_pos > 0
                || $l_pos > 0 || $r_pos > 0) {
            $this->calcHebe($wid, $vert);

            $hebe_h = $this->hebe_h;
            $hebe_v = $this->hebe_v;

            if ($t_pos) {
                $price += $unitprice * $hebe_h;
            }
            if ($b_pos) {
                $price += $unitprice * $hebe_h;
            }
            if ($l_pos) {
                $price += $unitprice * $hebe_v;
            }
            if ($r_pos) {
                $price += $unitprice * $hebe_v;
            }

        } else if ($t_pos === 0 && $b_pos === 0
                && $l_pos === 0 && $r_pos === 0) {
            $price = -1;
        }

        if ($t_cnt > 0 || $b_cnt > 0
                || $l_cnt > 0 || $r_cnt > 0) {

            $t_cnt = $t_cnt < 0 ? 0 : $t_cnt;
            $b_cnt = $b_cnt < 0 ? 0 : $b_cnt;
            $l_cnt = $l_cnt < 0 ? 0 : $l_cnt;
            $r_cnt = $r_cnt < 0 ? 0 : $r_cnt;

            $sum = $t_cnt + $b_cnt + $l_cnt + $r_cnt;
            $price = $sum * $unitprice;

        } else if ($t_cnt < 0 && $b_cnt < 0
                && $l_cnt < 0 && $r_cnt < 0) {
            $price = -1;
        }

        if (strpos($depth1, "사방") !== false) {
            $price = $unitprice;
        }

        if (empty($price)) {
            $price = $unitprice;
        }

        if ($price < 0) {
            $price = 0;
        }

        return $price * $amt;
    }

    /**
     * @brief 옵션 거치대 가격 계산
     *
     * @param $param = 가격 계산용 파라미터
     * @detail unitprice = 단가
     * depth2 = 거치대 depth2
     * amt = 단가에 곱할 수량
     *
     * @return 가격
     */
    public function calcRackPrice($param) {
        include_once(INC_PATH . '/define/front/product_ao_rack_info.inc');
        $depth1    = $param["depth1"];
        $depth2    = $param["depth2"];
        $unitprice = $param["unitprice"];
        $amt       = intval($param["amt"]);

        $unit = RACK_PACKAGE_UNIT[$depth2];
        $add_price = RACK_ADD_PRICE[$depth2];

        $box = 0;
        $ea  = 0;
        $price = 0;

        if (strpos($depth1, "실내용") !== false) {
            $box = intval($amt / $unit);
            $ea  = $amt % $unit;

            $price = ($box * $unit * $unitprice)
                     + ($ea * $add_price + $ea * $unitprice);
        } else {
            $box = $amt;

            $price = $amt * $unitprice;
        }

        return [
             "box"   => $box
            ,"ea"    => $ea
            ,"price" => $price
        ];
    }
}
