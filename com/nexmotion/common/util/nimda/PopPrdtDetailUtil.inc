<?
/*
 * Copyright (c) 2017 Nexmotion, Inc.
 * All rights reserved.
 *
 * 영업 - 주문관리 - 품목별 상세 현황 정보 팝업 html 생성 클래스
 *
 * REVISION HISTORY (reverse chronological order)
 *=============================================================================
 * 2017/06/07 엄준현 생성
 *=============================================================================
 */
class PopPrdtDetailUtil {
    private $conn;
    private $dao;
    
    private $span_up = "<span style=\"color:#5daff5\">▲</span>";
    private $span_down = "<span style=\"color:#ff0000\">▼</span>";

    public function __construct(array $param) {
        $this->conn = $param["conn"];
        $this->dao  = $param["dao"];
    }

    /**
     * @brief 상품 상세정보 tr html 생성
     *
     * @param $param    = 데이터 검색하는데 필요한 파라미터 배열
     * @param $date_arr = 일자 배열
     *
     * @returl 생성된 tr html
     */
    public function makePrdtDetailHtml(array $param, array $date_arr): string {
        $ret = '';

        $conn = $this->conn;
        $dao  = $this->dao;

        $date_ym_arr = array();

        $date_arr_count = count($date_arr);
        $flattyp_yn     = $param["flattyp_yn"];

        $temp = array();
        $temp["cate_sortcode"] = $param["cate_sortcode"];
        $temp["member_seqno"]  = $param["member_seqno"];

        $sort_arr = array();
        for ($i = 0; $i < $date_arr_count; $i++) {
            $temp["from"] = $date_arr[$i]["from"];
            $temp["to"]   = $date_arr[$i]["to"];

            $date_ym = substr($date_arr[$i]["from"], 0, -3);
            $date_ym_arr[] = $date_ym;

            $rs = $dao->selectPrdtDetailInfoSum($conn,
                                                $temp,
                                                $flattyp_yn);
            while ($rs && !$rs->EOF) {
                $fields = $rs->fields;

                $amt = $fields["amt"];
                $cnt = intval($fields["cnt"]);
                $pay_price = intval($fields["pay_price"]);

                $key = sprintf("%s|%s", $fields["typ"]
                                      , $fields["print_tmpt_name"]);

                if (empty($sort_arr[$key][$amt][$date_ym])) {
                    $sort_arr[$key][$amt][$date_ym]["cnt"]       = $fields["cnt"];
                    $sort_arr[$key][$amt][$date_ym]["pay_price"] = $fields["pay_price"];
                } else {
                    $sort_arr[$key][$amt][$date_ym]["cnt"]       += $fields["cnt"];
                    $sort_arr[$key][$amt][$date_ym]["pay_price"] += $fields["pay_price"];
                }

                $rs->MoveNext();
            }
        }

        // 증감률 계산
        $vary_arr = $this->calcVary($sort_arr,
                                    "cnt",
                                    $date_ym_arr);
        $vary_arr = $this->calcVary($sort_arr,
                                    "pay_price",
                                    $date_ym_arr,
                                    $vary_arr);

        // html form
        $tr_first_form = $this->getStartTrHtmlForm();
        $tr_form       = $this->getTrHtmlForm();

        $td_first_form = $this->getFirstTdHtmlForm();
        $td_amt_form   = $this->getAmtTdHtmlForm();
        $td_form       = $this->getTdHtmlForm();
        $td_sum_form   = $this->getSumTdHtmlForm();

        $ret = '';
        // 전체 rowspan 값
        $full_rowspan = 0;
        // 카테고리명 등 포함 여부
        $is_start = true;
        foreach ($sort_arr as $key => $amt_arr) {
            // 항목별 rowspan 값
            $key_rowspan = 0;
            // 소계용 배열
            $sum_arr = array();

            foreach ($amt_arr as $amt => $data_arr) {
                $vary_data = $vary_arr[$key . '|' . $amt];

                $td_html = '';
                // 수량값 포함 여부
                $is_first = true;
                for ($i = 0; $i < $date_arr_count; $i++) {
                    $date_ym = $date_ym_arr[$i];
                    $data    = $data_arr[$date_ym];
                    $vary    = $vary_data[$date_ym];

                    $cnt       = doubleval($data["cnt"]);
                    $pay_price = doubleval($data["pay_price"]);

                    $cnt_vary       = doubleval($vary["cnt"]);
                    $pay_price_vary = doubleval($vary["pay_price"]);

                    // 화살표 html 설정
                    $cnt_arrow = $this->span_up;
                    if ($cnt_vary < 0) {
                        $cnt_arrow = $this->span_down;
                    } else if (empty($cnt_vary)) {
                        $cnt_arrow = '';
                    }

                    $price_arrow = $this->span_up;
                    if ($pay_price_vary < 0) {
                        $price_arrow = $this->span_down;
                    } else if (empty($pay_price_vary)) {
                        $price_arrow = '';
                    }

                    // 도수별 구분값 설정
                    $key_arr = explode('|', $key);

                    if ($flattyp_yn === 'Y') {
                        $dvs = $key_arr[1];
                    } else {
                        $dvs = $key_arr[0] . "<br/>" . $key_arr[1];
                    }

                    // 실제 td html 생성
                    if ($is_start && $is_first) {
                        $is_first = false;
                        $td_html .= sprintf($td_first_form, $dvs
                                                          , $amt
                                                          , number_format($cnt)
                                                          , number_format($cnt_vary)
                                                          , $cnt_arrow
                                                          , number_format($pay_price)
                                                          , number_format($pay_price_vary)
                                                          , $price_arrow);
                    } else if (!$is_start && $is_first) {
                        $is_first = false;
                        $td_html .= sprintf($td_amt_form, $amt
                                                        , number_format($cnt)
                                                        , number_format($cnt_vary)
                                                        , $cnt_arrow
                                                        , number_format($pay_price)
                                                        , number_format($pay_price_vary)
                                                        , $price_arrow);
                    } else {
                        $td_html .= sprintf($td_form, number_format($cnt)
                                                    , number_format($cnt_vary)
                                                    , $cnt_arrow
                                                    , number_format($pay_price)
                                                    , number_format($pay_price_vary)
                                                    , $price_arrow);
                    }

                    // 소계용 데이터 입력
                    if (empty($sum_arr[$key][$amt][$date_ym]) === null) {
                        $sum_arr[$key][$amt][$date_ym]["cnt"] = 0;
                        $sum_arr[$key][$amt][$date_ym]["pay_price"] = 0;
                    } else {
                        $sum_arr[$key][$amt][$date_ym]["cnt"] += $cnt;
                        $sum_arr[$key][$amt][$date_ym]["pay_price"] += $pay_price;
                    }
                }

                if ($is_start) {
                    $is_start = false;
                    $ret .= sprintf($tr_first_form, $param["cate_name"]
                                                  , $td_html);
                } else {
                    $ret .= sprintf($tr_form, $td_html);
                }

                $key_rowspan++;
                $full_rowspan++;
            }
            $ret = str_replace("@2", $key_rowspan, $ret);

            $full_rowspan++;
            // 소계 증감률 계산
            $sum_vary_arr = $this->calcVary($sum_arr,
                                            "cnt",
                                            $date_ym_arr);
            $sum_vary_arr = $this->calcVary($sum_arr,
                                            "pay_price",
                                            $date_ym_arr,
                                            $vary_arr);
            // 소계 tr 생성
            $sum_arr = $sum_arr[$key][$amt];
            $sum_vary_arr = $sum_vary_arr[$key . '|' . $amt];

            $td_html = '';
            $is_first = true;
            for ($i = 0; $i < $date_arr_count; $i++) {
                $date_ym = $date_ym_arr[$i];
                $data    = $sum_arr[$date_ym];
                $vary    = $sum_vary_data[$date_ym];

                $cnt       = doubleval($data["cnt"]);
                $pay_price = doubleval($data["pay_price"]);

                $cnt_vary       = doubleval($vary["cnt"]);
                $pay_price_vary = doubleval($vary["pay_price"]);

                // 화살표 html 설정
                $cnt_arrow = $this->span_up;
                if ($cnt_vary < 0) {
                    $cnt_arrow = $this->span_down;
                } else if (empty($cnt_vary)) {
                    $cnt_arrow = '';
                }

                $price_arrow = $this->span_up;
                if ($pay_price_vary < 0) {
                    $price_arrow = $this->span_down;
                } else if (empty($pay_price_vary)) {
                    $price_arrow = '';
                }

                if ($is_first) {
                    $is_first = false;
                    $td_html .= sprintf($td_sum_form, "소계"
                                                    , number_format($cnt)
                                                    , number_format($cnt_vary)
                                                    , $cnt_arrow
                                                    , number_format($pay_price)
                                                    , number_format($pay_price_vary)
                                                    , $price_arrow);
                } else {
                    $td_html .= sprintf($td_form, number_format($cnt)
                                                , number_format($cnt_vary)
                                                , $cnt_arrow
                                                , number_format($pay_price)
                                                , number_format($pay_price_vary)
                                                , $price_arrow);
                }
            }
        }

        $ret .= sprintf($tr_form, $td_html);

        $ret = str_replace("@1", $full_rowspan, $ret);

        return $ret;
    }

    /**
     * @brief 상품 후공정 tr html 생성
     *
     * @param $param    = 데이터 검색하는데 필요한 파라미터 배열
     * @param $date_arr = 일자 배열
     *
     * @returl 생성된 tr html
     */
    public function makePrdtAfterHtml($param, $date_arr): string {
        $ret = '';

        $conn = $this->conn;
        $dao  = $this->dao;

        $date_ym_arr = array();
        
        return $ret;
    }

    /**
     * @brief 증감 계산
     *
     * @param $arr      = 정렬된 리스트 배열값
     * @param $dvs      = 정렬 리스트 배열값에서 값을 가져올 필드명
     * @param $date_arr = 날짜 배열
     * @param $ret      = 반환값 배열
     *
     * @return 기간별 증감값 배열
     */
    private function calcVary($arr, $dvs, $date_arr, $ret = array()): array {
        $cur       = $date_arr[0];
        $last_year = $date_arr[1];
        $m1        = $date_arr[2];
        $m2        = $date_arr[3];
        $m3        = $date_arr[4];

        foreach ($arr as $key => $amt_arr) {
            foreach ($amt_arr as $amt => $data_arr) {
                // 건수
                $cur_val       = doubleval($data_arr[$cur][$dvs]);
                $last_year_val = doubleval($data_arr[$last_year][$dvs]);
                $m1_val        = doubleval($data_arr[$m1][$dvs]);
                $m2_val        = doubleval($data_arr[$m2][$dvs]);
                $m3_val        = doubleval($data_arr[$m3][$dvs]);

                // 분모가 0일경우 전부 0으로 처리
                if (empty($last_year_val)) {
                    $cur_val_vary       = 0;
                } else {
                    $cur_val_vary       = $this->calcRate($cur_val, $last_year_val);
                }

                if (empty($cur_val)) {
                    $last_year_val_vary = 0;
                    $m1_val_vary        = 0;
                    $m2_val_vary        = 0;
                    $m3_val_vary        = 0;
                } else {
                    $last_year_val_vary = $this->calcRate($last_year_val, $cur_val);
                    $m1_val_vary        = $this->calcRate($m1_val, $cur_val);
                    $m2_val_vary        = $this->calcRate($m2_val, $cur_val);
                    $m3_val_vary        = $this->calcRate($m3_val, $cur_val);
                }

                $temp_key = $key . '|' . $amt;

                $ret[$temp_key][$cur][$dvs]       = $cur_val_vary;
                $ret[$temp_key][$last_year][$dvs] = $last_year_val_vary;
                $ret[$temp_key][$m1][$dvs]        = $m1_val_vary;
                $ret[$temp_key][$m2][$dvs]        = $m2_val_vary;
                $ret[$temp_key][$m3][$dvs]        = $m3_val_vary;
            }
        }

        return $ret;
    }

    /**
     * @brief 비율 계산
     *
     * @param $numerator = 분자
     * @param $denominator = 분모
     *
     * @return 비율
     */
    private function calcRate($numerator, $denominator): int {
        return ($numerator / $denominator) * 100;
    }

    private function getStartTrHtmlForm(): string {
        $html  = "<tr>";
        $html .=     "<td rowspan=\"@1\" style=\"background:#f5f5f5\">%s</td>"; // cate_name
        $html .=     "<td rowspan=\"@1\" style=\"background:#f5f5f5\">도수/수량</td>";
        $html .=     "%s";
        $html .= "</tr>";

        return $html;
    }

    private function getTrHtmlForm(): string {
        $html  = "<tr>";
        $html .=     "%s";
        $html .= "</tr>";

        return $html;
    }

    private function getFirstTdHtmlForm(): string {
        $html  =  "<td rowspan=\"@2\" style=\"background:#f5f5f5\">%s</td>"; // key
        $html .= "<td>%s</td>"; // amt
        $html .= "<td>%s</td>";
        $html .= "<td>%s&nbsp;%s</td>";
        $html .= "<td>%s</td>";
        $html .= "<td>%s&nbsp;%s</td>";

        return $html;
    }

    private function getAmtTdHtmlForm(): string {
        $html  = "<td>%s</td>"; // amt
        $html .= "<td>%s</td>";
        $html .= "<td>%s&nbsp;%s</td>";
        $html .= "<td>%s</td>";
        $html .= "<td>%s&nbsp;%s</td>";

        return $html;
    }

    private function getTdHtmlForm(): string {
        $html  = "<td>%s</td>";
        $html .= "<td>%s&nbsp;%s</td>";
        $html .= "<td>%s</td>";
        $html .= "<td>%s&nbsp;%s</td>";

        return $html;
    }

    private function getSumTdHtmlForm(): string {
        $html  = "<td colspan=\"2\">%s</td>";
        $html .= "<td>%s</td>";
        $html .= "<td>%s&nbsp;%s</td>";
        $html .= "<td>%s</td>";
        $html .= "<td>%s&nbsp;%s</td>";

        return $html;
    }
}
?>
