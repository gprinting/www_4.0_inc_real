<?
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/BusinessCommonDAO.inc');

class TempDAO extends BusinessCommonDAO {
    /**
     * @brief 카테고리 대분류 일련번호로 소분류까지 생성
     *
     * @param $conn       = connection identifier
     * @param $sortcode_t = 카테고리 대분류 분류코드
     *
     * @return 검색결과
     */
    function selectCateTMBHtml($conn, $sortcode_t) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $temp = array();
        $temp["col"] = "sortcode, cate_name";
        $temp["table"] = "cate";
        $temp["where"]["cate_level"] = '1';
        $temp["where"]["sortcode"] = $sortcode_t;
        
        $rs = $this->selectData($conn, $temp);
        unset($temp["where"]);

        $top_html = '';
        while ($rs && !$rs->EOF) {
            $fields = $rs->fields;

            $top_html .= option($fields["cate_name"], $fields["sortcode"]);

            $rs->MoveNext();
        }

        $temp["where"]["cate_level"] = '2';
        $temp["where"]["high_sortcode"] = $sortcode_t;

        $rs = $this->selectData($conn, $temp);

        $mid_html = '';
        $sortcode_m = '';
        while ($rs && !$rs->EOF) {
            $fields = $rs->fields;

            if (empty($mid_html)) {
                $sortcode_m = $fields["sortcode"];
            }

            $mid_html .= option($fields["cate_name"], $fields["sortcode"]);

            $rs->MoveNext();
        }

        $temp["col"] = "sortcode, cate_name, flattyp_yn";
        $temp["where"]["cate_level"] = '3';
        $temp["where"]["high_sortcode"] = $sortcode_m;

        $rs = $this->selectData($conn, $temp);

        $bot_html = '';
        $cate_name = '';
        $flattyp_yn = '';
        $cate_sortcode = '';
        while ($rs && !$rs->EOF) {
            $fields = $rs->fields;

            if (empty($bot_html)) {
                $cate_name = $fields["cate_name"];
                $flattyp_yn = $fields["flattyp_yn"];
                $cate_sortcode = $fields["sortcode"];
            }

            $attr = "flattyp_yn=\"" . $fields["flattyp_yn"] . "\"";

            $bot_html .= option($fields["cate_name"],
                                $fields["sortcode"],
                                $attr);

            $rs->MoveNext();
        }

        return array(
            "top" => $top_html,
            "mid" => $mid_html,
            "bot" => $bot_html,
            "cate_name"     => $cate_name,
            "flattyp_yn"    => $flattyp_yn,
            "cate_sortcode" => $cate_sortcode
        );
    }

    /**
     * @brief 카테고리 대분류 일련번호로 소분류까지 생성
     *
     * @param $conn       = connection identifier
     * @param $param      = 검색조건 파라미터
     * @param $flattyp_yn = 카테고리 낱장여부
     *
     * @return 검색결과
     */
    function selectPrdtDetailInfoSum($conn, $param, $flattyp_yn) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n SELECT  A.amt";
        $query .= "\n        ,A.count AS cnt";
        $query .= "\n        ,A.pay_price";
        $query .= "\n        ,B.typ";
        $query .= "\n        ,B.print_tmpt_name";

        $query .= "\n   FROM  order_common AS A";
        if ($flattyp_yn === 'Y') {
            $query .= "\n         ,order_detail AS B";
        } else {
            $query .= "\n         ,order_detail_brochure AS B";
        }
        $query .= "\n  WHERE  A.order_common_seqno = B.order_common_seqno";
        $query .= "\n    AND  A.member_seqno = %s";
        $query .= "\n    AND  %s 00:00:00' <= A.depo_finish_date";
        $query .= "\n    AND  A.depo_finish_date <= %s 23:59:59'";
        $query .= "\n    AND  A.cate_sortcode = %s";

        $query .= "\n  ORDER  BY A.amt + 0";

        $query  = sprintf($query, $param["member_seqno"]
                                , substr($param["from"], 0, -1)
                                , substr($param["to"], 0, -1)
                                , $param["cate_sortcode"]);

        return $conn->Execute($query);
    }
}
?>
