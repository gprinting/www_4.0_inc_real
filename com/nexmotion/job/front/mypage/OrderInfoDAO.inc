<?
include_once(INC_PATH . '/com/nexmotion/job/front/common/MypageCommonDAO.inc');
include_once(INC_PATH . '/com/nexmotion/html/front/mypage/OrderInfoHTML.inc');
include_once(INC_PATH . '/com/nexmotion/common/util/front/FrontCommonUtil.inc');

class OrderInfoDAO extends MypageCommonDAO {
    /**
     * @brief 주문취소시 UPDATE
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateOrderState($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    UPDATE  order_common";
        $query .= "\n       SET  order_state = %s";
        $query .= "\n           ,del_yn = 'Y'";
        $query .= "\n           ,eraser = %s";
        $query .= "\n     WHERE  order_common_seqno = %s";

        $query  = sprintf($query, $param["order_state"]
                                , $param["member_name"]
                                , $param["order_seqno"]);

        $result = $conn->Execute($query);

        if ($result === false) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 주문취소시 주문상세 상태값 UPDATE
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateOrderDetailState($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $table_name = $param["table_name"];

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    UPDATE  %s";
        $query .= "\n       SET  state = %s";
        $query .= "\n           ,del_yn = 'Y'";
        $query .= "\n     WHERE  order_common_seqno = %s";

        $query  = sprintf($query, $table_name
                                , $param["order_state"]
                                , $param["order_seqno"]);

        $result = $conn->Execute($query);

        if ($result === false) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 주문취소시 주문상세파일 상태값 UPDATE
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateOrderDetailCountFileState($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    UPDATE  order_detail_count_file";
        $query .= "\n       SET  state = %s";
        $query .= "\n     WHERE  order_detail_seqno = %s";

        $query  = sprintf($query, $param["order_state"]
                                , $param["order_detail_seqno"]);

        $result = $conn->Execute($query);

        if ($result === false) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 회원 주문부족금액 UPDATE(입금대기 시)
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateMemberOrderLackPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n    UPDATE  member";
        $query .= "\n       SET  order_lack_price = IFNULL(order_lack_price, 0) + %s";
        $query .= "\n     WHERE  member_seqno = %s";
        $query  = sprintf($query, $conn->param($param["order_lack_price"])
                                , $conn->param($param["member_seqno"]));

        $temp = [];
        $temp[] = $param["order_lack_price"];
        $temp[] = $param["member_seqno"];

        $result = $conn->Execute($query, $temp);

        if ($result === false) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 회원 이름 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberName($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n    SELECT  member_name";
        $query .= "\n      FROM  member";
        $query .= "\n     WHERE  member_seqno = %s";

        $query  = sprintf($query, $param["member_seqno"]);

        $result = $conn->Execute($query);

        return $result;

    }

    /**
     * @brief 주문상세 일련번호 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderDetailSeqno($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $table_name = $param["table_name"];

        $param = $this->parameterArrayEscape($conn, $param);
        $query .= "\n    SELECT  %s_seqno AS detail_seqno";
        $query .= "\n      FROM  %s";
        $query .= "\n     WHERE  order_common_seqno = %s";
        $query  = sprintf($query, $table_name
                                , $table_name
                                , $param["order_seqno"]);

        $result = $conn->Execute($query);

        return $result;
    }

    /**
     * @brief 주문 번호 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderNum($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query .= "\n    SELECT  order_num";
        $query .= "\n      FROM  order_common";
        $query .= "\n     WHERE  order_common_seqno = %s";
        $query  = sprintf($query, $param["order_seqno"]);

        $result = $conn->Execute($query);
        return $result->fields["order_num"];
    }

    /**
     * @brief 주문 번호 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderSeqno($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query .= "\n    SELECT  order_common_seqno";
        $query .= "\n      FROM  order_common";
        $query .= "\n     WHERE  order_num = %s";
        $query  = sprintf($query, $param["order_num"]);

        $result = $conn->Execute($query, [$param["order_num"]]);

        return $result->fields["order_common_seqno"];
    }

    /**
     * @brief 주문부족금액 0으로 UPDATE(주문취소 시, 입금대기 시)
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateOrderLackPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query .= "\n    UPDATE  order_common";
        $query .= "\n       SET  order_lack_price = 0";
        $query .= "\n           ,depo_price = 0";
        $query .= "\n     WHERE  1 = 1";
        $query .= "\n       AND  order_common_seqno = %s";
        $query .= "\n       AND  member_seqno       = %s";
        $query  = sprintf($query, $conn->param($param["order_common_seqno"])
                                , $conn->param($param["member_seqno"]));

        $bind = [];
        $bind[] = $param["order_common_seqno"];
        $bind[] = $param["member_seqno"];

        $result = $conn->Execute($query, $bind);

        if ($result === false) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 주문에 해당하는 후공정 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderAfter($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n    SELECT  after_name";
        $query .= "\n           ,depth1";
        $query .= "\n           ,depth2";
        $query .= "\n           ,depth3";
        $query .= "\n      FROM  order_after_history";
        $query .= "\n     WHERE  order_common_seqno = ";
        $query .= $param["order_seqno"];

        $result = $conn->Execute($query);

        return $result;
    }

    /**
     * @brief 주문취소를 위한 정보 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  ";
        $query .= "\n            A.order_common_seqno";
        $query .= "\n           ,A.add_after_price";
        $query .= "\n           ,A.add_opt_price";
        $query .= "\n           ,A.grade_sale_price";
        $query .= "\n           ,A.member_sale_price";
        $query .= "\n           ,A.depo_price";
        $query .= "\n           ,A.member_seqno";
        $query .= "\n           ,A.card_depo_price";
        $query .= "\n           ,A.pay_way";
        $query .= "\n           ,A.order_lack_price";
        $query .= "\n           ,A.use_point_price";
        $query .= "\n           ,A.order_state";
        $query .= "\n           ,A.title";
        $query .= "\n           ,A.order_num";
        $query .= "\n           ,A.dlvr_dvs";
        $query .= "\n           ,B.deal_num";
        $query .= "\n           ,B.sell_price";
        $query .= "\n           ,B.dvs";
        $query .= "\n           ,B.pay_price";
        $query .= "\n           ,C.bun_dlvr_order_num";
        $query .= "\n           ,D.sell_channel";
        $query .= "\n      FROM  order_common AS A ";
        $query .= "\n      INNER JOIN member_pay_history AS B ON A.order_num = B.order_num ";
        $query .= "\n      INNER JOIN order_dlvr AS C ON C.order_common_seqno = A.order_common_seqno AND C.tsrs_dvs = '수신' ";
        $query .= "\n      INNER JOIN member AS D ON A.member_seqno = D.member_seqno ";
        $query .= "\n     WHERE  A.order_common_seqno = %s";

        $query  = sprintf($query, $param["order_seqno"]);

        $result = $conn->Execute($query);

        return $result;

    }

    function selectCountBunDelivery($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT COUNT(*) cnt FROM order_dlvr AS A  ";
        $query .= "\n    INNER JOIN order_common AS B ON A.order_common_seqno = B.order_common_seqno ";
        $query .= "\n    WHERE A.bun_dlvr_order_num = %s AND B.member_seqno = %s AND A.tsrs_dvs = '수신' ";
        $query .= "\n    AND B.order_state != '1180' ";

        $query  = sprintf($query, $param["bun_dlvr_order_num"], $param["member_seqno"]);
        return $conn->Execute($query)->fields["cnt"];

    }

    function updateDeliveryFeeToAnotherRow($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    UPDATE member_pay_history AS A   ";
        $query .= "\n    INNER JOIN member_pay_history AS B ON A.member_seqno = B.member_seqno AND A.dvs = B.dvs AND B.pay_price = 0 AND A.deal_date = B.deal_date ";
        $query .= "\n    INNER JOIN order_common AS C ON B.order_num = C.order_num AND C.dlvr_dvs = 'namecard' AND C.order_state NOT IN ('1080', '1120','1180','1220') AND C.OPI_Date IS NULL ";
        $query .= "\n    SET B.sell_price = A.sell_price, B.pay_price = A.pay_price, A.sell_price = 0, A.pay_price = 0 ";
        $query .= "\n    WHERE A.order_num = %s AND A.dvs = '배송비' AND A.pay_price != 0 ";
        $query .= "\n    LIMIT 1 ";

        $query  = sprintf($query, $param["order_num"]);
        $conn->Execute($query);
    }

    function selectBunDeliveryPrice($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT MAX(B.sell_price) pay_price ";
        $query .= "\n      FROM  order_common AS A ";
        $query .= "\n      INNER JOIN member_pay_history AS B ON A.order_num = B.order_num ";
        $query .= "\n      INNER JOIN order_dlvr AS C ON C.order_common_seqno = A.order_common_seqno AND C.tsrs_dvs = '수신' ";
        $query .= "\n     WHERE  C.bun_dlvr_order_num = %s AND A.member_seqno = %s AND dvs = '배송비' ";

        $query  = sprintf($query, $param["bun_dlvr_order_num"], $param["member_seqno"]);
        return $conn->Execute($query)->fields["pay_price"];
    }

    /**
     * @brief 배송지변경을 위한 정보 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderInfoForDlvr($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n    SELECT  A.order_state";
        $query .= "\n           ,A.pay_price";
        $query .= "\n           ,A.bun_yn";
        $query .= "\n           ,A.order_num";
        $query .= "\n           ,A.pay_way";
        $query .= "\n           ,B.invo_num";
        $query .= "\n           ,B.dlvr_way";
        $query .= "\n           ,B.addr";
        $query .= "\n           ,B.addr_detail";
        $query .= "\n      FROM  order_common AS A";
        $query .= "\n      INNER JOIN order_dlvr AS B ON A.order_common_seqno = B.order_common_seqno AND B.tsrs_dvs = '수신' ";
        $query .= "\n     WHERE  A.order_common_seqno = %s";

        $query  = sprintf($query, $param["order_seqno"]);

        $result = $conn->Execute($query);

        return $result;

    }

    /**
     * @brief 배송지변경을 위한 정보 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderInfoForDlvrChange($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  A.dlvr_way";
        $query .= "\n           ,A.dlvr_sum_way";
        $query .= "\n           ,MAX(A.dlvr_price) dlvr_price";
        $query .= "\n           ,A.name";
        $query .= "\n           ,A.recei";
        $query .= "\n           ,A.tel_num";
        $query .= "\n           ,A.cell_num";
        $query .= "\n           ,A.zipcode";
        $query .= "\n           ,A.addr";
        $query .= "\n           ,A.addr_detail";
        $query .= "\n           ,A.invo_num";
        $query .= "\n           ,A.bun_dlvr_order_num";
        $query .= "\n           ,B.cate_sortcode";
        $query .= "\n           ,B.expec_weight";
        $query .= "\n      FROM  order_dlvr A";
        $query .= "\n      INNER JOIN order_common B ON A.order_common_seqno = B.order_common_seqno";
        $query .= "\n     WHERE  1 = 1";
        $query .= "\n       AND  A.order_common_seqno = %s";
        $query .= "\n       AND  A.tsrs_dvs = '수신'";

        $query  = sprintf($query, $param["order_seqno"]);
        $result = $conn->Execute($query);

        return $result;

    }

    /**
     * @brief 직배회원 유무 판별
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectDirectDlvrYn($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  A.direct_dlvr_yn";
        $query .= "\n      FROM  member A";
        $query .= "\n     WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $param["member_seqno"]);

        $result = $conn->Execute($query);

        return $result;

    }

    function selectTogetherDelivery($conn, $bun_dlvr_order_num) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\n    SELECT  group_concat(CONCAT(order_common_seqno) SEPARATOR '|') as order_common_group";
        $query .= "\n      FROM  order_dlvr A";
        $query .= "\n     WHERE  A.bun_dlvr_order_num = '%s' AND A.tsrs_dvs = '수신' ";

        $query  = sprintf($query, $bun_dlvr_order_num);
        $result = $conn->Execute($query);

        return $result;

    }

    /**
     * @brief 직배회원 직배정보 검색
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectDirectDlvrInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  A.zipcode";
        $query .= "\n          , A.addr";
        $query .= "\n          , A.addr_detail";
        $query .= "\n          , A.tel_num";
        $query .= "\n          , A.cell_num";
        $query .= "\n          , A.member_name";
        $query .= "\n      FROM  member A";
        $query .= "\n     WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $param["member_seqno"]);

        $result = $conn->Execute($query);

        return $result;

    }

    /**
     * @brief 배송지변경 UPDATE
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateMemberDlvrOnProgress($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query =  " UPDATE order_dlvr AS A  \n";
        $query .= " INNER JOIN order_common B ON A.order_common_seqno = B.order_common_seqno \n";
        $query .= " INNER JOIN order_dlvr C ON A.bun_dlvr_order_num = C.bun_dlvr_order_num AND A.name = C.name  \n";
        $query .= "\n       SET  C.addr         = %s";
        $query .= "\n           ,C.addr_detail  = %s";
        $query .= "\n           ,C.zipcode      = %s";
        $query .= "\n           ,C.dlvr_way     = %s";
        $query .= "\n           ,C.dlvr_sum_way = %s";
        $query .= "\n           ,C.name         = %s";
        $query .= "\n           ,C.recei        = %s";
        $query .= "\n           ,C.tel_num      = %s";
        $query .= "\n           ,C.cell_num     = %s";
        $query .= "\n           ,C.invo_cpn     = %s";
        $query .= "\n           ,C.dlvr_price   = %s";
        $query .= "\n     WHERE  B.order_common_seqno = %s";
        $query .= "\n       AND  C.tsrs_dvs = '수신'";

        $query  = sprintf($query, $param["addr"]
                                , $param["addr_detail"]
                                , $param["zipcode"]
                                , $param["dlvr_way"]
                                , $param["dlvr_sum_way"]
                                , $param["name"]
                                , $param["recei"]
                                , $param["tel_num"]
                                , $param["cell_num"]
                                , $param["invo_cpn"]
                                , $param["dlvr_price"]
                                , $param["order_seqno"]);

        $result = $conn->Execute($query);

        if ($result === false) {
            return false;
        } else {
            return true;
        }
    }


    /**
     * @brief 배송지변경 UPDATE
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateMemberPayDlvr($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $is_init = $param["is_init"];
        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    UPDATE order_common AS A ";
        $query .= "\n    INNER JOIN order_dlvr AS B ON A.order_common_seqno = B.order_common_seqno AND B.tsrs_dvs = '수신' ";
        $query .= "\n    INNER JOIN order_dlvr AS C ON B.bun_dlvr_order_num = C.bun_dlvr_order_num ";
        $query .= "\n    INNER JOIN order_common AS D ON D.order_common_seqno = C.order_common_seqno ";
        $query .= "\n    INNER JOIN member_pay_history AS E ON E.order_num = D.order_num AND E.dvs = '배송비' ";
        $query .= "\n    SET E.sell_price = %s, E.pay_price = %s, C.dlvr_price = %s ";
        $query .= "\n    WHERE A.order_common_seqno = %s ";

        if(!$is_init) {
            $query .= "\n    ORDER BY A.order_num LIMIT 2 ";
        }

        $query  = sprintf($query
            ,$param["sell_price"]
            ,$param["pay_price"]
            ,$param["dlvr_price"]
            ,$param["order_common_seqno"]);

        $result = $conn->Execute($query);

        if ($result === false) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 배송묶음을 위한 정보 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderBindInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param, ["order_seqno"=>true]);

        $query .= "\n    SELECT  A.member_seqno";
        $query .= "\n           ,A.cate_sortcode";
        $query .= "\n           ,A.order_detail";
        $query .= "\n           ,A.order_state";
        $query .= "\n           ,B.bun_dlvr_order_num";
        $query .= "\n           ,B.bun_group";
        $query .= "\n           ,B.addr";
        $query .= "\n           ,B.addr_detail";
        $query .= "\n           ,B.name";
        $query .= "\n      FROM  order_common AS A";
        $query .= "\n LEFT JOIN  order_dlvr AS B";
        $query .= "\n        ON  A.order_common_seqno = B.order_common_seqno";
        $query .= "\n     WHERE  A.order_common_seqno IN(" . $param["order_seqno"] . ")";
        $query .= "\n       AND  B.tsrs_dvs = '수신'";

        $result = $conn->Execute($query);
        return $result;

    }

    /**
     * @brief 배송묶음을 위한 카테고리 정보 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectCateSortcode($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param, ["cate_name"=>true]);

        $query .= "\n    SELECT  sortcode";
        $query .= "\n      FROM  cate";
        $query .= "\n     WHERE  cate_name IN(" . $param["cate_name"] . ")";

        $result = $conn->Execute($query);
        return $result;

    }

    /**
     * @brief 배송묶음 UPDATE
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateDlvrOrderNum($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param, ["order_seqno"=>true]);
        $query  = "\n    UPDATE  order_dlvr";
        $query .= "\n       SET  bun_dlvr_order_num = " . $param["bun_dlvr_order_num"];
        $query .= "\n           ,bun_group          = " . $param["bun_group"];
        $query .= "\n     WHERE  order_common_seqno IN(" . $param["order_seqno"] .")";

        $result = $conn->Execute($query);

        if ($result === false) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 재주문을 위한 배송 정보 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectDlvrInfoDup($conn, $param, $flag) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n    SELECT";
        $query .= "\n            order_dlvr_seqno";
        $query .= "\n           ,tsrs_dvs";
        $query .= "\n           ,name";
        $query .= "\n           ,recei";
        $query .= "\n           ,tel_num";
        $query .= "\n           ,cell_num";
        $query .= "\n           ,zipcode";
        $query .= "\n           ,addr";
        $query .= "\n           ,addr_detail";
        $query .= "\n           ,sms_yn";
        $query .= "\n           ,dlvr_way";
        $query .= "\n           ,dlvr_price";
        $query .= "\n           ,dlvr_sum_way";
        $query .= "\n           ,invo_cpn";
        $query .= "\n           ,invo_num";
        $query .= "\n           ,bun_group";
        $query .= "\n           ,lump_count";
        $query .= "\n      FROM  order_dlvr";
        $query .= "\n     WHERE  order_common_seqno = ";
        $query .= $param["order_common_seqno"];
        if ($flag == "송신") {
            $query .= "\n       AND  tsrs_dvs = '송신'";
        } else if ($flag == "수신") {
            $query .= "\n       AND  tsrs_dvs = '수신'";
        }

        $rs = $conn->Execute($query);

        return $rs->fields;
    }

    /**
     * @brief 주문파일을 위한 정보 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderFileSet($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n    SELECT  dvs";
        $query .= "\n           ,file_path";
        $query .= "\n           ,save_file_name";
        $query .= "\n           ,origin_file_name";
        $query .= "\n           ,size";
        $query .= "\n           ,order_file_seqno";
        $query .= "\n      FROM  order_file";
        $query .= "\n     WHERE  order_common_seqno = ";
        $query .= $param["order_seqno"];

        $result = $conn->Execute($query);

        return $result;

    }

    /**
     * @brief 주문파일을 위한 정보 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateReuploadOrder($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    UPDATE order_common ";
        $query .= "\n    SET file_upload_dvs = 'R' ";
        $query .= "\n     WHERE  order_common_seqno = ";
        $query .= $param["order_seqno"];

        $result = $conn->Execute($query);

        return $result;

    }

    function insertOrderInfoHistory($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n INSERT INTO order_info_history ";
        $query .= "\n       (order_common_seqno, kind, before_detail, after_detail, empl_id) VALUES (%s, %s, %s, %s, %s)";

        $query  = sprintf($query, $param["order_common_seqno"], $param["kind"], $param["before"], $param["after"], $param["empl_id"]);
        return $conn->Execute($query);
    }

    /**
     * @brief 재주문 order_common INSERT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertReorder($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $check = TRUE;

        $query  = "\n INSERT INTO order_common (";
        $query .= "\n                         receipt_mng";
        $query .= "\n                         ,order_num";
        $query .= "\n                         ,order_state";
        $query .= "\n                         ,pay_price";
        $query .= "\n                         ,page_cnt";
        $query .= "\n                         ,count";
        $query .= "\n                         ,receipt_start_date";
        $query .= "\n                         ,receipt_finish_date";
        $query .= "\n                         ,member_seqno";
        $query .= "\n                         ,order_mng";
        $query .= "\n                         ,title";
        $query .= "\n                         ,group_seqno";
        $query .= "\n                         ,cpn_admin_seqno";
        $query .= "\n                         ,oper_sys";
        $query .= "\n                         ,order_detail";
        $query .= "\n                         ,bun_group";
        $query .= "\n                         ,mono_yn";
        $query .= "\n                         ,claim_yn";
        $query .= "\n                         ,point_use_yn";
        $query .= "\n                         ,event_yn";
        $query .= "\n                         ,use_point_price";
        $query .= "\n                         ,grade_sale_price";
        $query .= "\n                         ,member_sale_price";
        $query .= "\n                         ,add_after_price";
        $query .= "\n                         ,add_opt_price";
        $query .= "\n                         ,event_price";
        $query .= "\n                         ,cp_price";
        $query .= "\n                         ,sell_price";
        $query .= "\n                         ,depo_price";
        $query .= "\n                         ,card_depo_price";
        $query .= "\n                         ,order_lack_price";
        $query .= "\n                         ,order_regi_date";
        $query .= "\n                         ,depo_finish_date";
        $query .= "\n                         ,expec_weight";
        $query .= "\n                         ,cust_memo";
        $query .= "\n                         ,work_memo";
        $query .= "\n                         ,del_yn";
        $query .= "\n                         ,eraser";
        $query .= "\n                         ,owncompany_img_num";
        $query .= "\n                         ,pay_way";
        $query .= "\n                         ,cate_sortcode";
        $query .= "\n                         ,opt_use_yn";
        $query .= "\n                         ,expenevid_req_yn";
        $query .= "\n                         ,expenevid_dvs";
        $query .= "\n                         ,expenevid_num";
        $query .= "\n                         ,stor_release_yn";
        $query .= "\n                         ,bun_yn";
        $query .= "\n                         ,dlvr_finish_date";
        $query .= "\n                         ,file_upload_dvs";
        $query .= "\n                         ,receipt_dvs";
        $query .= "\n                         ,amt_unit_dvs";
        $query .= "\n                         ,amt";
        $query .= "\n                         ,dlvr_produce_dvs";
        $query .= "\n                         ,receipt_num";
        $query .= "\n                         ,qc_check_pc";
        $query .= "\n                         ,commerce_dvs";
        $query .= "\n                         ,dlvr_dvs";
        $query .= "\n                         ,expec_release_date";
        $query .= "\n ) SELECT ";
        $query .= "\n                          '' ";
        $query .= "\n     ,CONCAT(" .$param["new_order_num"].", LPAD((SELECT COUNT(*) + 1 FROM order_common AS B WHERE B.order_regi_date like '".date("Y-m-d")."%'), 5, '0'))";
        $query .= "\n                         , 1320";
        $query .= "\n                         ," . $param["price"];
        $query .= "\n                         ," . $param["amt"];
        $query .= "\n                         ," . $param["count"];
        $query .= "\n                         ,NULL ";
        $query .= "\n                         ,NULL ";
        $query .= "\n                         ,member_seqno";
        $query .= "\n                         ,order_mng";
        $query .= "\n                         ,title";
        $query .= "\n                         ,group_seqno";
        $query .= "\n                         ,cpn_admin_seqno";
        $query .= "\n                         ,oper_sys";
        $query .= "\n                         ,order_detail";
        $query .= "\n                         ,bun_group";
        $query .= "\n                         ,mono_yn";
        $query .= "\n                         ,claim_yn";
        $query .= "\n                         ,point_use_yn";
        $query .= "\n                         ,event_yn";
        $query .= "\n                         ,use_point_price";
        $query .= "\n                         ,grade_sale_price";
        $query .= "\n                         ,member_sale_price";
        $query .= "\n                         ,add_after_price";
        $query .= "\n                         ,add_opt_price";
        $query .= "\n                         ,event_price";
        $query .= "\n                         ,cp_price";
        $query .= "\n                         ,sell_price";
        $query .= "\n                         ,depo_price";
        $query .= "\n                         ,card_depo_price";
        $query .= "\n                         ,order_lack_price";
        $query .= "\n                         ,NOW()";
        $query .= "\n                         ,NOW()";
        $query .= "\n                         ,expec_weight";
        $query .= "\n                         ,cust_memo";
        $query .= "\n                         ,work_memo";
        $query .= "\n                         ,del_yn";
        $query .= "\n                         ,eraser";
        $query .= "\n                         ,owncompany_img_num";
        $query .= "\n                         ,pay_way";
        $query .= "\n                         ,cate_sortcode";
        $query .= "\n                         ,opt_use_yn";
        $query .= "\n                         ,expenevid_req_yn";
        $query .= "\n                         ,expenevid_dvs";
        $query .= "\n                         ,expenevid_num";
        $query .= "\n                         ,stor_release_yn";
        $query .= "\n                         ,bun_yn";
        $query .= "\n                         ,dlvr_finish_date";
        $query .= "\n                         ,file_upload_dvs";
        $query .= "\n                         ,'Manual'";
        $query .= "\n                         ,amt_unit_dvs";
        $query .= "\n                         ,amt";
        $query .= "\n                         ,dlvr_produce_dvs";
        $query .= "\n                         ,receipt_num";
        $query .= "\n                         ,qc_check_pc";
        $query .= "\n                         ,commerce_dvs";
        $query .= "\n                         ,dlvr_dvs";
        $query .= "\n                         ,expec_release_date";
        $query .= "\n FROM order_common ";
        $query .= "\n WHERE order_num = " . $param["order_num"];

        $resultSet = $conn->Execute($query);

        if ($resultSet === false) {
            $check = false;
        }

        return $check;

    }

    /**
     * @brief 재주문 order_detail을 INSERT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertReworkOrderDetail($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $check = TRUE;

        $query  = "\n INSERT INTO order_detail (";
        $query .= "\n                         amt";
        $query .= "\n                         ,count";
        $query .= "\n                         ,receipt_mng";
        $query .= "\n                         ,pay_price";
        $query .= "\n                         ,receipt_start_date";
        $query .= "\n                         ,receipt_finish_date";
        $query .= "\n                         ,state";
        $query .= "\n                         ,order_detail_dvs_num";
        $query .= "\n                         ,order_common_seqno";

        $query .= "\n                         ,cate_sortcode";
        $query .= "\n                         ,typ";
        $query .= "\n                         ,paper_price";
        $query .= "\n                         ,output_price";
        $query .= "\n                         ,print_price";
        $query .= "\n                          ,paper_sum_price";
        $query .= "\n                         ,output_sum_price";
        $query .= "\n                         ,print_sum_price";
        $query .= "\n                         ,sell_price";
        $query .= "\n                         ,grade_sale_price";
        $query .= "\n                         ,member_sale_price";
        $query .= "\n                         ,use_point_price";
        $query .= "\n                         ,add_after_price";
        $query .= "\n                         ,cp_price";
        $query .= "\n                         ,del_yn";
        $query .= "\n                         ,work_size_wid";
        $query .= "\n                         ,work_size_vert";
        $query .= "\n                         ,cut_size_wid";
        $query .= "\n                         ,cut_size_vert";
        $query .= "\n                         ,receipt_size_wid";
        $query .= "\n                         ,receipt_size_vert";
        $query .= "\n                         ,tomson_size_wid";
        $query .= "\n                         ,tomson_size_vert";
        $query .= "\n                         ,cate_paper_mpcode";
        $query .= "\n                         ,cate_paper_tot_mpcode";
        $query .= "\n                         ,cate_beforeside_print_mpcode";
        $query .= "\n                         ,cate_beforeside_add_print_mpcode";
        $query .= "\n                         ,cate_aftside_print_mpcode";
        $query .= "\n                         ,cate_aftside_add_print_mpcode";
        $query .= "\n                         ,cate_output_mpcode";
        $query .= "\n                         ,order_detail";
        $query .= "\n                         ,mono_yn";
        $query .= "\n                         ,stan_name";
        $query .= "\n                         ,print_tmpt_name";
        $query .= "\n                         ,spc_dscr";
        $query .= "\n                         ,print_purp_dvs";
        $query .= "\n                         ,tot_tmpt";
        $query .= "\n                         ,page_amt";
        $query .= "\n                         ,amt_unit_dvs";
        $query .= "\n                         ,expec_weight";
        $query .= "\n                         ,after_use_yn";
        $query .= "\n                         ,receipt_memo";
        $query .= "\n                         ,side_dvs";
        $query .= "\n                         ,tomson_yn";
        $query .= "\n                         ,produce_memo";
        $query .= "\n                         ,typset_way";
        $query .= "\n ) SELECT ";
        $query .= "\n                         " . $param["amt"];
        $query .= "\n                         ," . $param["count"];
        $query .= "\n                         ,'' ";
        $query .= "\n                         , 0";
        $query .= "\n                         , NULL";
        $query .= "\n                         , NULL";
        $query .= "\n                         , 1320";
        $query .= "\n                         ," . $param["order_detail_dvs_num"];
        $query .= "\n                         ," . $param["order_seqno"];
        $query .= "\n                         ,cate_sortcode";
        $query .= "\n                         ,typ";
        $query .= "\n                         ,paper_price";
        $query .= "\n                         ,output_price";
        $query .= "\n                         ,print_price";
        $query .= "\n                          ,paper_sum_price";
        $query .= "\n                         ,output_sum_price";
        $query .= "\n                         ,print_sum_price";
        $query .= "\n                         ,sell_price";
        $query .= "\n                         ,grade_sale_price";
        $query .= "\n                         ,member_sale_price";
        $query .= "\n                         ,use_point_price";
        $query .= "\n                         ,add_after_price";
        $query .= "\n                         ,cp_price";
        $query .= "\n                         ,del_yn";
        $query .= "\n                         ,work_size_wid";
        $query .= "\n                         ,work_size_vert";
        $query .= "\n                         ,cut_size_wid";
        $query .= "\n                         ,cut_size_vert";
        $query .= "\n                         ,receipt_size_wid";
        $query .= "\n                         ,receipt_size_vert";
        $query .= "\n                         ,tomson_size_wid";
        $query .= "\n                         ,tomson_size_vert";
        $query .= "\n                         ,cate_paper_mpcode";
        $query .= "\n                         ,cate_paper_tot_mpcode";
        $query .= "\n                         ,cate_beforeside_print_mpcode";
        $query .= "\n                         ,cate_beforeside_add_print_mpcode";
        $query .= "\n                         ,cate_aftside_print_mpcode";
        $query .= "\n                         ,cate_aftside_add_print_mpcode";
        $query .= "\n                         ,cate_output_mpcode";
        $query .= "\n                         ,order_detail";
        $query .= "\n                         ,mono_yn";
        $query .= "\n                         ,stan_name";
        $query .= "\n                         ,print_tmpt_name";
        $query .= "\n                         ,spc_dscr";
        $query .= "\n                         ,print_purp_dvs";
        $query .= "\n                         ,tot_tmpt";
        $query .= "\n                         ,page_amt";
        $query .= "\n                         ,amt_unit_dvs";
        $query .= "\n                         ,expec_weight";
        $query .= "\n                         ,after_use_yn";
        $query .= "\n                         ,receipt_memo";
        $query .= "\n                         ,side_dvs";
        $query .= "\n                         ,tomson_yn";
        $query .= "\n                         ,produce_memo";
        $query .= "\n                         ,typset_way";
        $query .= "\n FROM order_detail ";
        $query .= "\n WHERE order_common_seqno = " . $param["old_order_seqno"];

        $resultSet = $conn->Execute($query);

        if ($resultSet === false) {
            $check = false;
        }

        return $check;

    }

    /**
     * @brief 재주문 order_detail을 INSERT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertReworkOrderFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $check = TRUE;

        $query  = "\n INSERT INTO order_file (";
        $query .= "\n                         accept_file_path";
        $query .= "\n                         ,accept_file_name";
        $query .= "\n                         ,preview_file_path";
        $query .= "\n                         ,preview_file_name";

        $query .= "\n                         ,dvs";
        $query .= "\n                         ,file_path";
        $query .= "\n                         ,save_file_name";
        $query .= "\n                         ,origin_file_name";
        $query .= "\n                         ,size";
        $query .= "\n                         ,order_common_seqno";
        $query .= "\n                         ,member_seqno";
        $query .= "\n                         ,empl_name";
        $query .= "\n                         ,upload_time";
        $query .= "\n ) SELECT ";
        $query .= "\n                         NULL";
        $query .= "\n                         ,NULL";
        $query .= "\n                         ,NULL";
        $query .= "\n                         ,NULL";

        $query .= "\n                         ,dvs";
        $query .= "\n                         ,file_path";
        $query .= "\n                         ,save_file_name";
        $query .= "\n                         ,origin_file_name";
        $query .= "\n                         ,size";
        $query .= "\n                         ," . $param["order_seqno"];;
        $query .= "\n                         ,member_seqno";
        $query .= "\n                         ,NULL";
        $query .= "\n                         ,upload_time";
        $query .= "\n FROM order_file ";
        $query .= "\n WHERE order_common_seqno = " . $param["old_order_seqno"];

        $resultSet = $conn->Execute($query);

        if ($resultSet === false) {
            $check = false;
        }

        return $check;

    }

    /**
     * @brief 재작업/재주문내역 INSERT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertReworkList($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $check = TRUE;

        $query  = "\n INSERT INTO rework_list (";
        $query .= "\n                          order_common_seqno";
        $query .= "\n                         ,rework_order_common_seqno";
        $query .= "\n                         ,rework_cause";
        $query .= "\n                         ,rework_cause_detail";
        $query .= "\n                         ,empl_seqno";
        $query .= "\n                         ,request_empl_name";
        $query .= "\n) VALUES (";
        $query .= "\n            " . $param["old_order_seqno"];
        $query .= "\n           ," . $param["order_seqno"];
        $query .= "\n           ," . $param["rework_cause"];
        $query .= "\n           ," . $param["rework_cause_detail"];
        $query .= "\n           ," . $param["rework_empl_id"];
        $query .= "\n           ," . $param["rework_request_empl"];
        $query .= "\n )";

        $resultSet = $conn->Execute($query);

        if ($resultSet === false) {
            $check = false;
        }

        return $check;

    }


    /**
     * @brief 재주문 주문파일을 INSERT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertOrderFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $check = TRUE;

        $query  = "\n INSERT INTO order_file (";
        $query .= "\n                          dvs";
        $query .= "\n                         ,file_path";
        $query .= "\n                         ,save_file_name";
        $query .= "\n                         ,origin_file_name";
        $query .= "\n                         ,size";
        $query .= "\n                         ,order_common_seqno";
        $query .= "\n                         ,member_seqno";
        $query .= "\n) VALUES (";
        $query .= "\n            " . $param["dvs"];
        $query .= "\n           ," . $param["file_path"];
        $query .= "\n           ," . $param["save_file_name"];
        $query .= "\n           ," . $param["origin_file_name"];
        $query .= "\n           ," . $param["size"];
        $query .= "\n           ," . $param["order_common_seqno"];
        $query .= "\n           ," . $param["member_seqno"];
        $query .= "\n )";

        $resultSet = $conn->Execute($query);

        if ($resultSet === false) {
            $check = false;
        }

        return $check;

    }

    /**
     * @brief 주문상세를 위한 정보 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderDetailSet($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n    SELECT  typ";
        $query .= "\n           ,page_amt";
        $query .= "\n           ,cate_beforeside_print_mpcode";
        $query .= "\n           ,cate_beforeside_add_print_mpcode";
        $query .= "\n           ,cate_aftside_print_mpcode";
        $query .= "\n           ,cate_aftside_add_print_mpcode";
        $query .= "\n           ,cate_paper_mpcode";
        $query .= "\n           ,spc_dscr";
        $query .= "\n           ,order_detail_dvs_num";
        $query .= "\n           ,work_size_wid";
        $query .= "\n           ,work_size_vert";
        $query .= "\n           ,cut_size_wid";
        $query .= "\n           ,cut_size_vert";
        $query .= "\n           ,tomson_size_wid";
        $query .= "\n           ,tomson_size_vert";
        $query .= "\n           ,cut_front_wing_size_wid";
        $query .= "\n           ,cut_front_wing_size_vert";
        $query .= "\n           ,work_front_wing_size_wid";
        $query .= "\n           ,work_front_wing_size_vert";
        $query .= "\n           ,cut_rear_wing_size_wid";
        $query .= "\n           ,cut_rear_wing_size_vert";
        $query .= "\n           ,work_rear_wing_size_wid";
        $query .= "\n           ,work_rear_wing_size_vert";
        $query .= "\n           ,seneca_size";
        $query .= "\n      FROM  order_detail";
        $query .= "\n     WHERE  order_common_seqno = ";
        $query .= $param["order_seqno"];

        $result = $conn->Execute($query);

        return $result;

    }

    /**
     * @brief 재주문 주문상세 INSERT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertOrderDetail($conn, $result, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $order_num = substr($param["order_num"], 1, -1);

        $check = TRUE;
        $idx = 1;

        while ($result && !$result->EOF) {

            $query  = "\n INSERT INTO order_detail (";
            $query .= "\n                          cate_beforeside_print_mpcode";
            $query .= "\n                         ,cate_beforeside_add_print_mpcode";
            $query .= "\n                         ,cate_aftside_print_mpcode";
            $query .= "\n                         ,cate_aftside_add_print_mpcode";
            $query .= "\n                         ,typ";
            $query .= "\n                         ,page_amt";
            $query .= "\n                         ,cate_paper_mpcode";
            $query .= "\n                         ,spc_dscr";
            $query .= "\n                         ,order_detail_dvs_num";
            $query .= "\n                         ,work_size_wid";
            $query .= "\n                         ,work_size_vert";
            $query .= "\n                         ,cut_size_wid";
            $query .= "\n                         ,cut_size_vert";
            $query .= "\n                         ,tomson_size_wid";
            $query .= "\n                         ,tomson_size_vert";
            $query .= "\n                         ,cut_front_wing_size_wid";
            $query .= "\n                         ,cut_front_wing_size_vert";
            $query .= "\n                         ,work_front_wing_size_wid";
            $query .= "\n                         ,work_front_wing_size_vert";
            $query .= "\n                         ,cut_rear_wing_size_wid";
            $query .= "\n                         ,cut_rear_wing_size_vert";
            $query .= "\n                         ,work_rear_wing_size_wid";
            $query .= "\n                         ,work_rear_wing_size_vert";
            $query .= "\n                         ,seneca_size";
            $query .= "\n                         ,order_detail_num";
            $query .= "\n                         ,order_common_seqno";
            $query .= "\n                         ,del_yn";
            $query .= "\n) VALUES (";
            $query .= "\n          '" . $result->fields["cate_beforeside_print_mpcode"] . "'";
            $query .= "\n         ,'" . $result->fields["cate_beforeside_add_print_mpcode"] . "'";
            $query .= "\n         ,'" . $result->fields["cate_aftside_print_mpcode"] . "'";
            $query .= "\n         ,'" . $result->fields["cate_aftside_add_print_mpcode"] . "'";
            $query .= "\n         ,'" . $result->fields["typ"] . "'";
            $query .= "\n         ,'" . $result->fields["page_amt"] . "'";
            $query .= "\n         ,'" . $result->fields["cate_paper_mpcode"];
            $query .= "'";
            $query .= "\n         ,'" . $result->fields["spc_dscr"] . "'";
            $query .= "\n         ,'" . $result->fields["detail_num"] . "'";
            $query .= "\n         ,'" . $result->fields["work_size_wid"];
            $query .= "'";
            $query .= "\n         ,'" . $result->fields["work_size_vert"];
            $query .= "'";
            $query .= "\n         ,'" . $result->fields["cut_size_wid"];
            $query .= "'";
            $query .= "\n         ,'" . $result->fields["cut_size_vert"];
            $query .= "'";
            $query .= "\n         ,'" . $result->fields["tomson_size_wid"];
            $query .= "'";
            $query .= "\n         ,'" . $result->fields["tomson_size_vert"];
            $query .= "'";
            $query .= "\n         ,'" . $result->fields["cut_front_wing_size_wid"] . "'";
            $query .= "\n         ,'" . $result->fields["cut_front_wing_size_vert"] . "'";
            $query .= "\n         ,'" . $result->fields["work_front_wing_size_wid"] . "'";
            $query .= "\n         ,'" . $result->fields["work_front_wing_size_vert"] . "'";
            $query .= "\n         ,'" . $result->fields["cut_rear_wing_size_wid"] . "'";
            $query .= "\n         ,'" . $result->fields["cut_rear_wing_size_vert"] . "'";
            $query .= "\n         ,'" . $result->fields["work_rear_wing_size_wid"] . "'";
            $query .= "\n         ,'" . $result->fields["work_rear_wing_size_vert"] . "'";
            $query .= "\n         ,'" . $result->fields["seneca_size"] . "'";
            $query .= "\n         ,'" . $order_num . $idx . "'";
            $query .= "\n         , " . $param["reorder_seqno"];
            $query .= "\n         , 'N'";
            $query .= "\n )";

            $resultSet = $conn->Execute($query);
            if ($resultSet === false) {
                $check = false;
            }

            $result->moveNext();
            $idx++;
        }

        return $check;

    }

    /**
     * @brief 재주문 주문상세 INSERT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertReworkOrderAfterHistory($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $check = TRUE;

        $query  = "\n INSERT INTO order_after_history (";
        $query .= "\n                          order_common_seqno";
        $query .= "\n                         ,order_detail_dvs_num";

        $query .= "\n                         ,basic_yn";
        $query .= "\n                         ,mpcode";
        $query .= "\n                         ,after_name";
        $query .= "\n                         ,depth1";
        $query .= "\n                         ,depth2";
        $query .= "\n                         ,depth3";
        $query .= "\n                         ,detail";
        $query .= "\n                         ,info";
        $query .= "\n                         ,price";
        $query .= "\n                         ,seq";
        $query .= "\n ) SELECT ";
        $query .= "\n                          " . $param["order_seqno"];
        $query .= "\n                         ," . $param["order_detail_dvs_num"];

        $query .= "\n                         ,basic_yn";
        $query .= "\n                         ,mpcode";
        $query .= "\n                         ,after_name";
        $query .= "\n                         ,depth1";
        $query .= "\n                         ,depth2";
        $query .= "\n                         ,depth3";
        $query .= "\n                         ,detail";
        $query .= "\n                         ,info";
        $query .= "\n                         ,price";
        $query .= "\n                         ,seq";
        $query .= "\n FROM order_after_history ";
        $query .= "\n WHERE order_common_seqno = " . $param["old_order_seqno"];

        $resultSet = $conn->Execute($query);

        if ($resultSet === false) {
            $check = false;
        }

        return $check;
    }

    /**
     * @brief 재주문 주문상세 INSERT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertReworkOrderOptHistory($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $check = TRUE;

        $query  = "\n INSERT INTO order_opt_history (";
        $query .= "\n                          order_common_seqno";

        $query .= "\n                         ,basic_yn";
        $query .= "\n                         ,mpcode";
        $query .= "\n                         ,opt_name";
        $query .= "\n                         ,depth1";
        $query .= "\n                         ,depth2";
        $query .= "\n                         ,depth3";
        $query .= "\n                         ,detail";
        $query .= "\n                         ,info";
        $query .= "\n                         ,price";
        $query .= "\n ) SELECT ";
        $query .= "\n                          " . $param["order_seqno"];

        $query .= "\n                         ,basic_yn";
        $query .= "\n                         ,mpcode";
        $query .= "\n                         ,opt_name";
        $query .= "\n                         ,depth1";
        $query .= "\n                         ,depth2";
        $query .= "\n                         ,depth3";
        $query .= "\n                         ,detail";
        $query .= "\n                         ,info";
        $query .= "\n                         ,price";
        $query .= "\n FROM order_opt_history ";
        $query .= "\n WHERE order_common_seqno = " . $param["old_order_seqno"];

        $resultSet = $conn->Execute($query);

        if ($resultSet === false) {
            $check = false;
        }

        return $check;
    }

    /**
     * @brief 재주문 주문상세 INSERT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertReworkOrderDelivery($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $check = TRUE;

        $query  = "\n INSERT INTO order_dlvr (";
        $query .= "\n                          order_common_seqno";

        $query .= "\n                         ,tsrs_dvs";
        $query .= "\n                         ,name";
        $query .= "\n                         ,recei";
        $query .= "\n                         ,tel_num";
        $query .= "\n                         ,cell_num";
        $query .= "\n                         ,zipcode";
        $query .= "\n                         ,addr";
        $query .= "\n                         ,addr_detail";
        $query .= "\n                         ,sms_yn";
        $query .= "\n                         ,dlvr_way";
        $query .= "\n                         ,dlvr_price";
        $query .= "\n                         ,dlvr_sum_way";
        $query .= "\n                         ,invo_num";
        $query .= "\n                         ,invo_cpn";
        $query .= "\n                         ,bun_dlvr_order_num";
        $query .= "\n                         ,bun_group";
        $query .= "\n                         ,lump_count";
        $query .= "\n                         ,release_date";
        $query .= "\n                         ,dlvr_req";

        $query .= "\n ) SELECT ";
        $query .= "\n                          " . $param["order_seqno"];

        $query .= "\n                         ,tsrs_dvs";
        $query .= "\n                         ,name";
        $query .= "\n                         ,recei";
        $query .= "\n                         ,tel_num";
        $query .= "\n                         ,cell_num";
        $query .= "\n                         ,zipcode";
        $query .= "\n                         ,addr";
        $query .= "\n                         ,addr_detail";
        $query .= "\n                         ,sms_yn";
        $query .= "\n                         ,dlvr_way";
        $query .= "\n                         ,dlvr_price";
        $query .= "\n                         ,dlvr_sum_way";
        $query .= "\n                         ,invo_num";
        $query .= "\n                         ,invo_cpn";
        $query .= "\n                         ,bun_dlvr_order_num";
        $query .= "\n                         ,bun_group";
        $query .= "\n                         ,lump_count";
        $query .= "\n                         ,release_date";
        $query .= "\n                         ,dlvr_req";
        $query .= "\n FROM order_dlvr ";
        $query .= "\n WHERE order_common_seqno = " . $param["old_order_seqno"];

        $resultSet = $conn->Execute($query);

        if ($resultSet === false) {
            $check = false;
        }

        return $check;
    }

    /**
     * @brief 재주문을 위한 후공정 정보 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderAfterSet($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n    SELECT  basic_yn";
        $query .= "\n           ,after_name";
        $query .= "\n           ,depth1";
        $query .= "\n           ,depth2";
        $query .= "\n           ,depth3";
        $query .= "\n           ,price";
        $query .= "\n           ,seq";
        $query .= "\n           ,detail";
        $query .= "\n      FROM  order_after_history";
        $query .= "\n     WHERE  order_common_seqno = ";
        $query .= $param["order_seqno"];

        $result = $conn->Execute($query);

        return $result;

    }

    /**
     * @brief 재주문 후공정 INSERT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertOrderAfter($conn, $result, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $check = TRUE;

        while ($result && !$result->EOF) {

            $query  = "\n INSERT INTO ";
            $query .= "\n order_after_history (";
            $query .= "\n                       basic_yn";
            $query .= "\n                      ,depth1";
            $query .= "\n                      ,depth2";
            $query .= "\n                      ,depth3";
            $query .= "\n                      ,price";
            $query .= "\n                      ,after_name";
            $query .= "\n                      ,seq";
            $query .= "\n                      ,detail";
            $query .= "\n                      ,order_common_seqno";
            $query .= "\n) VALUES (";
            $query .= "\n            '" . $result->fields["basic_yn"] . "'";
            $query .= "\n           ,'" . $result->fields["depth1"] . "'";
            $query .= "\n           ,'" . $result->fields["depth2"] . "'";
            $query .= "\n           ,'" . $result->fields["depth3"] . "'";
            $query .= "\n           ,'" . $result->fields["price"] . "'";
            $query .= "\n           ,'" . $result->fields["after_name"] . "'";
            $query .= "\n           ,'" . $result->fields["seq"] . "'";
            $query .= "\n           ,'" . $result->fields["detail"] . "'";
            $query .= "\n           , " . $param["reorder_seqno"];
            $query .= "\n )";


            $resultSet = $conn->Execute($query);
            if ($resultSet === false) {
                $check = false;
            }

            $result->moveNext();
        }

        return $check;

    }

    /**
     * @brief 재주문을 위한 옵션 정보 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderOptSet($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n    SELECT  basic_yn";
        $query .= "\n           ,opt_name";
        $query .= "\n           ,depth1";
        $query .= "\n           ,depth2";
        $query .= "\n           ,depth3";
        $query .= "\n           ,price";
        $query .= "\n      FROM  order_opt_history";
        $query .= "\n     WHERE  order_common_seqno = ";
        $query .= $param["order_seqno"];

        $result = $conn->Execute($query);

        return $result;

    }

    /**
     * @brief 재주문 옵션 INSERT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function insertOrderOpt($conn, $result, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $check = TRUE;

        while ($result && !$result->EOF) {

            $query  = "\n INSERT INTO ";
            $query .= "\n order_opt_history (";
            $query .= "\n                       basic_yn";
            $query .= "\n                      ,depth1";
            $query .= "\n                      ,depth2";
            $query .= "\n                      ,depth3";
            $query .= "\n                      ,price";
            $query .= "\n                      ,opt_name";
            $query .= "\n                      ,order_common_seqno";
            $query .= "\n) VALUES (";
            $query .= "\n            '" . $result->fields["basic_yn"] . "'";
            $query .= "\n           ,'" . $result->fields["depth1"] . "'";
            $query .= "\n           ,'" . $result->fields["depth2"] . "'";
            $query .= "\n           ,'" . $result->fields["depth3"] . "'";
            $query .= "\n           ,'" . $result->fields["price"] . "'";
            $query .= "\n           ,'" . $result->fields["opt_name"] . "'";
            $query .= "\n           , " . $param["reorder_seqno"];
            $query .= "\n )";

            $resultSet = $conn->Execute($query);
            if ($resultSet === false) {
                $check = false;
            }

            $result->moveNext();
        }

        return $check;

    }


    /**
     * @brief 주문 메모 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectOrderMemo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n    SELECT  cust_memo";
        $query .= "\n      FROM  order_common";
        $query .= "\n     WHERE  order_common_seqno = %s";
        $query .= "\n       AND  member_seqno = %s";

        $query  = sprintf($query, $param["order_seqno"]
                                , $param["member_seqno"]);

        $result = $conn->Execute($query);

        return $result;

    }

    /**
     * @brief 주문메모 UPDATE
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function updateOrderMemo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);
        $query  = "\n    UPDATE  order_common";
        $query .= "\n       SET  cust_memo = " . $param["cust_memo"];
        $query .= "\n     WHERE  order_common_seqno = " . $param["order_seqno"];

        $result = $conn->Execute($query);

        if ($result === false) {
            return false;
        } else {
            return true;
        }
    }

    /**
     * @brief 회원 등급 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectMemberInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n    SELECT  grade";
        $query .= "\n           ,prepay_price_money";
        $query .= "\n           ,prepay_price_card";
        $query .= "\n           ,order_lack_price";
        $query .= "\n      FROM  member";
        $query .= "\n     WHERE  member_seqno = ";
        $query .= $param["member_seqno"];

        $result = $conn->Execute($query);

        return $result;

    }

    /**
     * @brief 등급별 할인율 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectGradeRate($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\n    SELECT  sales_sale_rate";
        $query .= "\n      FROM  member_grade_policy";
        $query .= "\n     WHERE  grade = ";
        $query .= $param["grade"];

        $result = $conn->Execute($query);

        return $result;

    }

    /**
     * @brief 관심상품 리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPrdtList($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $s_num = $param["s_num"];
        $list_num = $param["list_num"];

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n    SELECT  /* [OrderInfoDAO.inc] 관심상품 리스트 검색 */";
        $query .= "\n            SQL_CALC_FOUND_ROWS";
        $query .= "\n            A.interest_prdt_seqno";
        $query .= "\n           ,A.title";
        $query .= "\n           ,A.regi_date";
        $query .= "\n           ,A.order_detail";
        $query .= "\n           ,A.amt";
        $query .= "\n           ,A.amt_unit_dvs";
        $query .= "\n           ,A.count";
        $query .= "\n      FROM  interest_prdt A";
        $query .= "\n     WHERE  A.member_seqno = %s";

        $query  = sprintf($query, $param["member_seqno"]);

        if ($this->blankParameterCheck($param ,"interest_prdt_seqno")) {
            $query .= "\n    AND  A.interest_prdt_seqno = ";
            $query .= $param["interest_prdt_seqno"];
        }
        if ($this->blankParameterCheck($param ,"title")) {
            $detail = substr($param["title"], 1, -1);
            $query .= "\n    AND  A.title LIKE '%" . $detail . "%'";
        }

        if (isset($s_num, $list_num)) {
            $query .= "\n ORDER BY A.regi_date DESC ";
            $query .= "\n LIMIT ". $s_num . ", " . $list_num;
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 관심상품에 해당하는 후공정 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPrdtAfter($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query .= "\n SELECT  after_name AS name";
        $query .= "\n        ,depth1";
        $query .= "\n        ,depth2";
        $query .= "\n        ,depth3";
        $query .= "\n        ,basic_yn";
        $query .= "\n        ,mpcode";
        $query .= "\n        ,info";
        $query .= "\n        ,interest_prdt_detail_dvs_num";
        $query .= "\n   FROM  interest_prdt_after_history";
        $query .= "\n  WHERE  interest_prdt_seqno = %s";
        $query .= "\n    AND  basic_yn = 'N'";

        $query  = sprintf($query, $conn->param($param["interest_prdt_seqno"]));

        $temp = [];
        $temp[] = $param["interest_prdt_seqno"];

        $result = $conn->Execute($query, $temp);

        return $result;
    }

    /**
     * @brief 관심상품에 해당하는 옵션 SELECT
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPrdtOpt($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query .= "\n SELECT  opt_name AS name";
        $query .= "\n        ,depth1";
        $query .= "\n        ,depth2";
        $query .= "\n        ,depth3";
        $query .= "\n        ,basic_yn";
        $query .= "\n        ,mpcode";
        $query .= "\n        ,detail";
        $query .= "\n   FROM  interest_prdt_opt_history";
        $query .= "\n  WHERE  interest_prdt_seqno = %s";
        $query .= "\n    AND  basic_yn = 'N'";

        $query  = sprintf($query, $conn->param($param["interest_prdt_seqno"]));

        $temp = [];
        $temp[] = $param["interest_prdt_seqno"];

        $result = $conn->Execute($query, $temp);

        return $result;
    }

     /**
      * @brief 현재 주문의 카테고리 분류코드 검색
      *
      * @param $conn  = connection identifier
      * @param $seqno = 주문공통 일련번호
      *
      * @return 카테고리 일련번호
      */
     function selectOrderCateSortcode($conn, $seqno) {
         if ($this->connectionCheck($conn) === false) {
             return false;
         }

         //$seqno = $this->parameterEscape($conn, $seqno);

         $temp = [];
         $temp["col"]   = "cate_sortcode";
         $temp["table"] = "order_common";
         $temp["where"]["order_common_seqno"] = $seqno;

         $rs = $this->selectData($conn, $temp);

         return $rs->fields["cate_sortcode"];
     }

    function updateOrderDlvrInfo1($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $query = "\n    UPDATE order_dlvr ";
        $query .= "\n   SET invo_num = '%s' ";
        $query .= "\n   , bun_dlvr_order_num = '%s' ";
        $query .= "\n   , invo_cpn = '%s' ";
        $query .= "\n   WHERE order_common_seqno = %s ";

        $bun_dlvr_order_num = strval(ceil(microtime(true) * 1000.0)) . rand(0, 9);

        $query = sprintf($query,
            $param['change_invo_num'],
            $bun_dlvr_order_num,
            $param['invo_kind'],
            $param['order_common_seqno']);

        $conn->Execute($query);
    }

    function updateOrderDlvrInfo2($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        $query =  " UPDATE order_dlvr AS A  \n";
        $query .= " INNER JOIN order_common B ON A.order_common_seqno = B.order_common_seqno \n";
        $query .= " INNER JOIN order_dlvr C ON A.bun_dlvr_order_num = C.bun_dlvr_order_num AND A.name = C.name  \n";
        $query .= " SET C.invo_num = '%s', invo_kind = '%s' \n";
        $query .= " WHERE B.order_common_seqno = '%s' \n";
        $query = sprintf($query, $param['invo_num'], $param['invo_kind'], $param['order_common_seqno']);

        $bun_dlvr_order_num = strval(ceil(microtime(true) * 1000.0)) . rand(0, 9);

        $query = sprintf($query,
            $param['change_invo_num'],
            $bun_dlvr_order_num,
            $param['order_common_seqno']);

        $conn->Execute($query);
    }
     /**
      * @brief 낱장 주문의 미리보기 파일 검색
      *
      * @param $conn  = connection identifier
      * @param $seqno = 주문공통 일련번호
      *
      * @return 카테고리 일련번호
      */
     function selectSheetPreviewPath($conn, $seqno) {
         if ($this->connectionCheck($conn) === false) {
             return false;
         }

         $query .= "\n SELECT  C.preview_file_path AS file_path";
         $query .= "\n        ,C.preview_file_name AS file_name";
         $query .= "\n   FROM  order_detail AS A";
         $query .= "\n        ,order_detail_count_file AS B";
         $query .= "\n        ,order_detail_count_preview_file AS C";
         $query .= "\n  WHERE  A.order_common_seqno = %s";
         $query .= "\n    AND  A.order_detail_seqno = B.order_detail_seqno";
         $query .= "\n    AND  B.order_detail_count_file_seqno = C.order_detail_count_file_seqno";
         $query  = sprintf($query, $conn->param($seqno));

         $bind = [];
         $bind[] = $seqno;

         $rs = $conn->Execute($query, $bind);

         return $rs;
     }

     /**
      * @brief 책자 주문의 미리보기 파일 검색
      *
      * @param $conn  = connection identifier
      * @param $seqno = 주문공통 일련번호
      *
      * @return 카테고리 일련번호
      */
     function selectBrochurePreviewPath($conn, $seqno) {
         if ($this->connectionCheck($conn) === false) {
             return false;
         }

         $query .= "\n SELECT  A.file_path      AS file_path";
         $query .= "\n        ,A.save_file_name AS file_name";
         $query .= "\n   FROM  order_detail AS A";
         $query .= "\n  WHERE  A.brochure_preview_file = %s";
         $query  = sprintf($query, $conn->param($seqno));

         $bind = [];
         $bind[] = $seqno;

         $rs = $conn->Execute($query, $bind);

         return $rs;
     }

     /**
      * @brief 카테고리 분류코드 검색 하면서 관심상품 데이터값 존재하는지 확인
      *
      * @param $conn  = connection identifier
      * @param $param = 검색조건 파라미터
      *
      * @return 검색 row 수
      */
     function selectInterestPrdtCateSortcode($conn, $param) {
         if (!$this->connectionCheck($conn)) {
             return false;
         }

         $query  = "\n SELECT  /* [OrderInfoDAO.inc] 관심상품 데이터 존재하는지 확인 */";
         $query .= "\n         A.cate_sortcode";
         $query .= "\n        ,A.title";
         $query .= "\n   FROM  interest_prdt AS A";
         $query .= "\n  WHERE  A.member_seqno = %s";
         $query .= "\n    AND  A.interest_prdt_seqno = %s";

         $query  = sprintf($query, $conn->param($param["member_seqno"])
                                 , $conn->param($param["interest_prdt_seqno"]));

         $temp = [];
         $temp[] = $param["member_seqno"];
         $temp[] = $param["interest_prdt_seqno"];

         $rs = $conn->Execute($query, $temp);

         //return $rs->fields["cate_sortcode"];
         return $rs;
     }

     /**
      * @brief 관심상품 상세 검색
      *
      * @param $conn  = connection identifier
      * @param $param = 검색조건 파라미터
      *
      * @return 검색결과
      */
     function selectInterestPrdtDetail($conn, $param) {
         if (!$this->connectionCheck($conn)) {
             return false;
         }

         $query .= "\n SELECT  /* [OrderInfoDAO.inc] 관심상품 낱장형 상세데이터 검색 */";
         $query .= "\n         A.cate_sortcode";
         $query .= "\n        ,A.work_size_wid";
         $query .= "\n        ,A.work_size_vert";
         $query .= "\n        ,A.cut_size_wid";
         $query .= "\n        ,A.cut_size_vert";
         $query .= "\n        ,A.tomson_size_wid";
         $query .= "\n        ,A.tomson_size_vert";
         $query .= "\n        ,A.cate_paper_tot_mpcode";
         $query .= "\n        ,A.cate_beforeside_print_mpcode";
         $query .= "\n        ,A.cate_beforeside_add_print_mpcode";
         $query .= "\n        ,A.cate_aftside_print_mpcode";
         $query .= "\n        ,A.cate_aftside_add_print_mpcode";
         $query .= "\n        ,A.cate_output_mpcode";
         $query .= "\n        ,A.stan_name";
         $query .= "\n        ,A.amt";
         $query .= "\n        ,A.count";
         $query .= "\n        ,A.order_detail";
         $query .= "\n        ,A.print_purp_dvs";
         $query .= "\n   FROM  interest_prdt_detail AS A";
         $query .= "\n  WHERE  A.interest_prdt_seqno = %s";

         $query  = sprintf($query, $conn->param($param["interest_prdt_seqno"]));

         $temp = [];
         $temp[] = $param["interest_prdt_seqno"];

         $rs = $conn->Execute($query, $temp);

         return $rs;
     }

     /**
      * @brief 관심상품 상세 검색
      *
      * @param $conn  = connection identifier
      * @param $param = 검색조건 파라미터
      *
      * @return 검색결과
      */
     function selectInterestPrdtDetailBrochure($conn, $param) {
         if (!$this->connectionCheck($conn)) {
             return false;
         }

         $query  = "\n SELECT  /* [OrderInfoDAO.inc] 관심상품 책자형 상세데이터 검색 */";
         $query .= "\n         A.cate_sortcode";
         $query .= "\n        ,A.interest_prdt_detail_dvs_num";
         $query .= "\n        ,A.typ";
         $query .= "\n        ,A.work_size_wid";
         $query .= "\n        ,A.work_size_vert";
         $query .= "\n        ,A.cut_size_wid";
         $query .= "\n        ,A.cut_size_vert";
         $query .= "\n        ,A.tomson_size_wid";
         $query .= "\n        ,A.tomson_size_vert";
         $query .= "\n        ,A.cut_front_wing_size_wid";
         $query .= "\n        ,A.cut_front_wing_size_vert";
         $query .= "\n        ,A.work_front_wing_size_wid";
         $query .= "\n        ,A.work_front_wing_size_vert";
         $query .= "\n        ,A.cut_rear_wing_size_wid";
         $query .= "\n        ,A.cut_rear_wing_size_vert";
         $query .= "\n        ,A.work_rear_wing_size_wid";
         $query .= "\n        ,A.work_rear_wing_size_vert";
         $query .= "\n        ,A.seneca_size";
         $query .= "\n        ,A.cate_paper_tot_mpcode";
         $query .= "\n        ,A.cate_beforeside_print_mpcode";
         $query .= "\n        ,A.cate_beforeside_add_print_mpcode";
         $query .= "\n        ,A.cate_aftside_print_mpcode";
         $query .= "\n        ,A.cate_aftside_add_print_mpcode";
         $query .= "\n        ,A.cate_output_mpcode";
         $query .= "\n        ,A.stan_name";
         $query .= "\n        ,A.page_amt";
         $query .= "\n        ,A.amt";
         $query .= "\n        ,A.print_purp_dvs";
         $query .= "\n   FROM  interest_prdt_detail_brochure AS A";
         $query .= "\n  WHERE  A.interest_prdt_seqno = %s";

         $query  = sprintf($query, $conn->param($param["interest_prdt_seqno"]));

         $temp = [];
         $temp[] = $param["interest_prdt_seqno"];

         $rs = $conn->Execute($query, $temp);

         return $rs;
     }

     /**
      * @brief 주문취소시 배송비 재계산용 정보검색
      *
      * @param $conn  = connection identifier
      * @param $param = 검색조건 파라미터
      *
      * @return 검색결과
      */
     function selectOrderDlvrPriceList($conn, $param) {
         if (!$this->connectionCheck($conn)) {
             return false;
         }

         $query  = "\n SELECT  /* [OrderInfoDAO.inc] 주문취소시 배송비 재계산용 정보검색 */";
         $query .= "\n         B.order_dlvr_seqno";
         $query .= "\n        ,B.dlvr_price";
         $query .= "\n        ,B.dlvr_way";
         $query .= "\n        ,B.zipcode";
         $query .= "\n        ,B.order_common_seqno";
         $query .= "\n        ,C.cate_sortcode";
         $query .= "\n        ,C.expec_weight";
         $query .= "\n   FROM  order_dlvr AS A";
         $query .= "\n        ,order_dlvr AS B";
         $query .= "\n        ,order_common AS C";
         $query .= "\n  WHERE  A.bun_dlvr_order_num = B.bun_dlvr_order_num";
         $query .= "\n    AND  B.order_common_seqno = C.order_common_seqno";
         $query .= "\n    AND  A.tsrs_dvs = '수신'";
         $query .= "\n    AND  B.tsrs_dvs = '수신'";
         $query .= "\n    AND  A.order_common_seqno = %s";

         $query  = sprintf($query, $conn->param($param["order_common_seqno"]));

         $rs = $conn->Execute($query, [$param["order_common_seqno"]]);
         return $rs;
     }
}
?>
