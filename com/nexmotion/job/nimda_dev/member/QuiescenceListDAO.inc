<?php
include_once(INC_PATH . "/com/nexmotion/job/nimda_dev/common/NimdaCommonDAO.inc");

class QuiescenceListDAO extends NimdaCommonDAO {
    /**
     * @brief 휴면대상회원리스트 검색
     *
     * @param $conn = db 커넥션
     * @param $param = 검색조건
     *
     * @return 검색결과
     */
    public function selectQuiescenceMemberList($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* [QuiescenceListDAO.inc] 휴면대상회원리스트 검색 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         member_seqno";
        $query .= "\n        ,member_name";
        $query .= "\n        ,mail";
        $query .= "\n        ,tel_num";
        $query .= "\n        ,cell_num";
        $query .= "\n        ,point";
        $query .= "\n        ,prepay_price_money";
        $query .= "\n        ,prepay_price_card";
        $query .= "\n        ,first_join_date";
        $query .= "\n        ,final_login_date";
        $query .= "\n   FROM  member";
        $query .= "\n  WHERE  1 = 1";
        if ($this->blankParameterCheck($param ,"quie_stan")) {
            $quie_stan = substr($param["quie_stan"], 1, -1);
            $query .= "\n    AND  (final_login_date < '" . $quie_stan . "'";
            $query .= "\n     OR  first_join_date < '" . $quie_stan . "')";
        }
        if ($this->blankParameterCheck($param ,"sell_channel")) {
            $query .= "\n    AND  sell_channel = ";
            $query .= $param["sell_channel"];
        }
        $query .= "\n  ORDER  BY member_seqno DESC";
        $query .= "\n  LIMIT  %s, %s";

        $query  = sprintf($query, substr($param["page"], 1, -1)
                                , substr($param["page_block"], 1, -1));

        $rs = $conn->Execute($query);

        return $rs;
    }
}
?>
