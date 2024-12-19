<?php
include_once(INC_PATH . "/com/nexmotion/job/nimda_dev/common/NimdaCommonDAO.inc");

class ReduceListDAO extends NimdaCommonDAO {
    /**
     * @brief 휴면대상회원리스트 검색
     *
     * @param $conn = db 커넥션
     * @param $param = 검색조건
     *
     * @return 검색결과
     */
    public function selectReduceMemberList($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  /* [ReduceListDAO.inc] 정리회원리스트 검색 */";
        if (!$this->blankParameterCheck($param, "page_dvs")) {
            $query .= "\n         SQL_CALC_FOUND_ROWS";
        }
        $query .= "\n         A.member_seqno";
        $query .= "\n        ,A.member_name";
        $query .= "\n        ,A.member_dvs";
        $query .= "\n        ,A.prepay_price_money";
        $query .= "\n        ,A.prepay_price_card";
        $query .= "\n        ,A.first_join_date";
        $query .= "\n        ,B.reason";
        $query .= "\n   FROM  member AS A";
        $query .= "\n        ,member_withdraw AS B";
        $query .= "\n  WHERE  A.member_seqno = B.member_seqno";
        if ($this->blankParameterCheck($param ,"member_name")) {
            $member_name = substr($param["member_name"], 1, -1);
            $query .= "\n    AND  member_name LIKE '%%";
            $query .= $member_name . "%%'";
        }
        if ($this->blankParameterCheck($param ,"member_dvs")) {
            $member_dvs = substr($param["member_dvs"], 1, -1);
            $query .= "\n    AND  member_dvs LIKE '%%";
            $query .= $member_dvs . "%%'";
        }
        if ($this->blankParameterCheck($param ,"first_join_date")) {
            $query .= "\n    AND  first_join_date BETWEEN ";
            $query .= $param["first_join_date"]["from"];
            $query .= " AND ";
            $query .= $param["first_join_date"]["to"];
        }
        if ($this->blankParameterCheck($param ,"final_order_date")) {
            $query .= "\n    AND  final_order_date BETWEEN %s";
            $query .= $param["final_order_date"]["from"];
            $query .= " AND ";
            $query .= $param["final_order_date"]["to"];
        }
        if ($this->blankParameterCheck($param ,"final_login_date")) {
            $query .= "\n    AND  final_login_date BETWEEN %s";
            $query .= $param["final_login_date"]["from"];
            $query .= " AND ";
            $query .= $param["final_login_date"]["to"];
        }
        if ($this->blankParameterCheck($param ,"sell_channel")) {
            $query .= "\n    AND  sell_channel = ";
            $query .= $param["sell_channel"];
        }
        if ($this->blankParameterCheck($param ,"grade")) {
            $query .= "\n    AND  grade = ";
            $query .= $param["grade"];
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
