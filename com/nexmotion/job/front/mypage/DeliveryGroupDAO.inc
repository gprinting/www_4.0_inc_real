<?php
include_once(INC_PATH . "/com/nexmotion/job/front/common/MypageCommonDAO.inc");

class DeliveryGroupDAO extends MypageCommonDAO {
    /**
     * @brief 회원 배송지 리스트
     *
     * @param $conn         = connection identifier
     * @param $member_seqno = 회원 일련번호
     *
     * @return 검색결과
     */
    public function selectOrderDlvrGroup($conn, $member_seqno) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [DeliveryGroupDAO.inc] - selectOrderDlvrGroup */";
        $query .= "\n         A.order_dlvr_seqno";
        $query .= "\n        ,A.name";
        $query .= "\n        ,A.recei";
        $query .= "\n        ,A.zipcode";
        $query .= "\n        ,A.addr";
        $query .= "\n        ,A.addr_detail";
        //$query .= "\n        ,A.dlvr_way";
        $query .= "\n        ,A.dlvr_sum_way";
        $query .= "\n        ,A.invo_cpn";
        $query .= "\n        ,A.bun_dlvr_order_num";
        //$query .= "\n        ,A.bun_group";
        $query .= "\n        ,A.lump_count";
        $query .= "\n        ,B.order_common_seqno";
        $query .= "\n        ,B.title";
        $query .= "\n        ,B.order_detail";
        $query .= "\n        ,B.cate_sortcode";
        $query .= "\n        ,B.amt";
        $query .= "\n        ,B.amt_unit_dvs";
        $query .= "\n        ,B.count";
        $query .= "\n        ,B.expec_weight";
        $query .= "\n        ,B.expec_release_date";
        $query .= "\n        ,B.order_state";
        $query .= "\n        ,B.bun_yn";
        $query .= "\n   FROM  order_dlvr   AS A";
        $query .= "\n        ,order_common AS B";
        $query .= "\n  WHERE  A.order_common_seqno = B.order_common_seqno";
        $query .= "\n    AND  A.tsrs_dvs = '수신'";
        $query .= "\n    AND  A.dlvr_way = '01'";
        $query .= "\n    AND  B.del_yn = 'N'";
        $query .= "\n    AND  B.order_state != '1180' AND B.order_state != '1080' AND B.OPI_Date IS NULL"; // 이거랑 입금대기도 막아야됨
        $query .= "\n    AND  B.order_state < '2320'";
        $query .= "\n    AND  (SUBSTR(B.cate_sortcode, 1, 3) = '003' ";
        $query .= "\n          OR SUBSTR(B.cate_sortcode, 1, 3) = '004'";
        $query .= "\n          OR SUBSTR(B.cate_sortcode, 1, 6) = '008002'";
        $query .= "\n          OR B.cate_sortcode = '008001001')";
        $query .= "\n    AND  B.member_seqno = %s";

        $query  = sprintf($query, $conn->param($member_seqno));

        $rs = $conn->Execute($query, [$member_seqno]);

        return $rs;
    }

    /**
     * @brief 묶음에 몇 개 들어가있는지 검색
     *
     * @param $conn     = connection identifier
     * @param $dlvr_num = 묶음배송주문번호
     *
     * @return 검색결과
     */
    public function selectOrderDlvrCount($conn, $dlvr_num) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [DeliveryGroupDAO.inc] - selectOrderDlvrCount */";
        $query .= "\n         COUNT(1) AS cnt";
        $query .= "\n   FROM  order_dlvr AS A";
        $query .= "\n  WHERE  A.bun_dlvr_order_num = %s";
        $query .= "\n    AND  A.tsrs_dvs = '수신'";

        $query  = sprintf($query, $conn->param($dlvr_num));

        $rs = $conn->Execute($query, [$dlvr_num]);

        return intval($rs->fields["cnt"]);
    }

    /**
     * @brief 배송비 입력되어 있는지 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 묶음배송주문번호, 주문공통일련번호
     *
     * @return 검색결과
     */
    public function selectOrderDlvrPrice($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [DeliveryGroupDAO.inc] - selectOrderDlvrPrice */";
        $query .= "\n         SUM(A.dlvr_price) AS dlvr_price";
        $query .= "\n   FROM  order_dlvr AS A";
        $query .= "\n  WHERE  A.bun_dlvr_order_num = %s";
        $query .= "\n    AND  A.order_common_seqno = %s";

        $query  = sprintf($query, $conn->param($param["bun_dlvr_order_num"])
                                , $conn->param($param["order_common_seqno"]));

        $temp = [
             $param["bun_dlvr_order_num"]
            ,$param["order_common_seqno"]
        ];

        $rs = $conn->Execute($query, $temp);

        return intval($rs->fields["dlvr_price"]);
    }

    /**
     * @brief 주문배송 정보 수정
     *
     * @param $conn  = connection identifier
     * @param $param = 묶음배송주문번호, 주문공통일련번호
     *
     * @return 검색결과
     */
    public function updateOrderDlvr($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n UPDATE  /* [DeliveryGroupDAO.inc] - updateOrderDlvr */";
        $query .= "\n         order_dlvr";
        $query .= "\n    SET  bun_dlvr_order_num = %s";
        $query .= "\n        ,dlvr_way           = %s";
        $query .= "\n        ,dlvr_sum_way       = %s";
        $query .= "\n        ,dlvr_price         = %s";
        $query .= "\n  WHERE  bun_dlvr_order_num = %s";
        $query .= "\n    AND  order_common_seqno = %s";

        $query  = sprintf($query, $conn->param($param["bun_dlvr_order_num"])
                                , $conn->param($param["dlvr_way"])
                                , $conn->param($param["dlvr_sum_way"])
                                , $conn->param($param["dlvr_price"])
                                , $conn->param($param["ex_bun_dlvr_order_num"])
                                , $conn->param($param["order_common_seqno"]));

        $temp = [
             $param["bun_dlvr_order_num"]
            ,$param["dlvr_way"]
            ,$param["dlvr_sum_way"]
            ,$param["dlvr_price"]
            ,$param["ex_bun_dlvr_order_num"]
            ,$param["order_common_seqno"]
        ];

        $rs = $conn->Execute($query, $temp);

        return $rs;
    }

    /**
     * @brief 수정한 주문의 배송비를 업데이트할 이전 묶음그룸 주문 하나 검색
     *
     * @param $conn   = connection identifier
     * @param $param = 묶음배송주문번호, 주문공통일련번호
     *
     * @return 검색결과
     */
    public function selectExOrderDlvr($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n SELECT  /* [DeliveryGroupDAO.inc] - selectExOrderDlvr */";
        $query .= "\n         order_dlvr_seqno";
        $query .= "\n        ,order_common_seqno";
        $query .= "\n   FROM  order_dlvr AS A";
        $query .= "\n  WHERE  A.bun_dlvr_order_num = %s";
        $query .= "\n    AND  A.order_common_seqno != %s";
        $query .= "\n    AND  A.tsrs_dvs = '수신'";
        $query .= "\n  LIMIT  1";

        $query  = sprintf($query, $conn->param($param["bun_dlvr_order_num"])
                                , $conn->param($param["order_common_seqno"]));

        $temp = [
             $param["bun_dlvr_order_num"]
            ,$param["order_common_seqno"]
        ];

        $rs = $conn->Execute($query, $temp);

        return $rs->fields;
    }


    /**
     * @brief 무게가 12kg 미만인 배송그룹정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 회원일련번호, 예상출고일
     *
     * @return 검색결과
     */
    public function selectOrderDlvrGroupInfo($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n   SELECT  /* [DeliveryGroupDAO.inc] - selectOrderDlvrGroupInfo */";
        $query .= "\n           SUM(A.expec_weight) AS sum_weight";
        $query .= "\n          ,B.name";
        $query .= "\n          ,B.recei";
        $query .= "\n          ,B.zipcode";
        $query .= "\n          ,B.addr";
        $query .= "\n          ,B.addr_detail";
        $query .= "\n          ,B.dlvr_way";
        $query .= "\n          ,B.dlvr_sum_way";
        $query .= "\n          ,B.invo_cpn";
        $query .= "\n          ,B.bun_dlvr_order_num";
        $query .= "\n     FROM  order_common AS A";
        $query .= "\n          ,order_dlvr   AS B";
        $query .= "\n    WHERE  A.order_common_seqno = B.order_common_seqno";
        $query .= "\n      AND  B.tsrs_dvs = '수신'";
        $query .= "\n      AND  A.bun_yn = 'Y'";
        $query .= "\n      AND  A.del_yn = 'N'";
        $query .= "\n      AND  A.order_state != '1180'"; // 이거랑 입금대기도 막아야됨
        $query .= "\n      AND  A.order_state < '2320'";
        $query .= "\n      AND  (SUBSTR(A.cate_sortcode, 1, 3) = '003' ";
        $query .= "\n            OR SUBSTR(A.cate_sortcode, 1, 3) = '004'";
        $query .= "\n            OR SUBSTR(A.cate_sortcode, 1, 6) = '008002'";
        $query .= "\n            OR A.cate_sortcode = '008001001')";
        $query .= "\n      AND  A.member_seqno = %s";
        $query .= "\n      AND  A.expec_release_date BETWEEN '%s 00:00:00' AND '%s 23:59:59'";
        $query .= "\n GROUP BY  B.bun_dlvr_order_num";
        $query .= "\n   HAVING  sum_weight < 12";
        $query .= "\n ORDER BY  NULL";

        $query  = sprintf($query, $conn->param($param["member_seqno"])
                                , $param["expec_release_date"]
                                , $param["expec_release_date"]);

        $rs = $conn->Execute($query, [$param["member_seqno"]]);

        return $rs;
    }

    /**
     * @brief 배송그룹에 추가할 상품의 정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 주문공통일련번호, 회원일련번호
     *
     * @return 검색결과
     */
    public function selectOrderDlvrInfo($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $query  = "\n   SELECT  /* [DeliveryGroupDAO.inc] - selectOrderDlvrInfo */";
        $query .= "\n           A.expec_release_date";
        $query .= "\n          ,A.expec_weight";
        $query .= "\n          ,A.order_num";
        $query .= "\n          ,B.order_dlvr_seqno";
        $query .= "\n          ,B.name";
        $query .= "\n          ,B.recei";
        $query .= "\n          ,B.zipcode";
        $query .= "\n          ,B.addr";
        $query .= "\n          ,B.addr_detail";
        $query .= "\n          ,B.dlvr_way";
        $query .= "\n          ,B.dlvr_sum_way";
        $query .= "\n          ,B.invo_cpn";
        $query .= "\n          ,B.bun_dlvr_order_num";
        $query .= "\n          ,B.dlvr_price";
        $query .= "\n     FROM  order_common AS A";
        $query .= "\n          ,order_dlvr   AS B";
        $query .= "\n    WHERE  A.order_common_seqno = B.order_common_seqno";
        $query .= "\n      AND  B.tsrs_dvs = '수신'";
        $query .= "\n      AND  A.order_common_seqno = %s";
        $query .= "\n      AND  A.member_seqno = %s";

        $query  = sprintf($query, $conn->param($param["order_common_seqno"])
                                , $conn->param($param["member_seqno"]));

        $temp = [
             $param["order_common_seqno"]
            ,$param["member_seqno"]
        ];

        $rs = $conn->Execute($query, $temp)->fields;

        return $rs;
    }

    /**
     * @brief 배송비 환불목록 입력
     *
     * @param $conn  = connection identifier
     * @param $param = 회원일련번호, 주문번호, 배송비
     *
     * @return 검색결과
     */
    public function insertOrderDlvrPriceRefund($conn, $param) {
        if (!$this->connectionCheck($conn)) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO /* [DeliveryGroupDAO.inc] - insertOrderDlvrPriceRefund */";
        $query .= "\n             order_dlvr_price_refund (";
        $query .= "\n      member_seqno";
        $query .= "\n     ,order_num";
        $query .= "\n     ,regi_date";
        $query .= "\n     ,dlvr_price";
        $query .= "\n     ,refund_yn";
        $query .= "\n ) VALUES (";
        $query .= "\n      %s";
        $query .= "\n     ,%s";
        $query .= "\n     ,now()";
        $query .= "\n     ,%s";
        $query .= "\n     ,'N'";
        $query .= "\n )";

        $query  = sprintf($query, $param["member_seqno"]
                                , $param["order_num"]
                                , $param["dlvr_price"]);

        $rs = $conn->Execute($query);

        return $rs;
    }
}
