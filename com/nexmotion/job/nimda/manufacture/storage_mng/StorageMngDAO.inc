<?
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/ManufactureCommonDAO.inc');
include_once(INC_PATH . "/com/nexmotion/html/nimda/manufacture/release_mng/ReleaseMngHtml.inc");

/**
 * @file PaperMaterialMngDAO.inc
 *
 * @brief 생산 - 자재관리 - 종이자재관리 DAO
 */
class StorageMngDAO extends ManufactureCommonDAO {

    /**
     * @brief 생성자
     *
     */
    function __construct() {
    }

    function selectDetailInfo($conn) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query  = "\nSELECT  opt_name, seq ";
        $query .= "\n   FROM  opt_info ";

        return $conn->Execute($query);
    }

    function selectStorageList($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $dvs = substr($param["dvs"], 1, -1);

        if ($dvs == "COUNT") {
            $query  = "\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT  D.member_name ";
            $query .= "\n       ,A.order_num ";
            $query .= "\n       ,A.order_regi_date ";
            $query .= "\n       ,A.depo_finish_date ";
            $query .= "\n       ,A.title ";
            $query .= "\n       ,B.order_detail ";
            $query .= "\n       ,B.amt ";
            $query .= "\n       ,B.count ";
            $query .= "\n       ,C.dlvr_way ";
            $query .= "\n       ,A.order_state ";
            $query .= "\n       ,B.order_detail_dvs_num ";
            $query .= "\n       ,B.order_common_seqno ";
            $query .= "\n       ,A.member_seqno ";
        } else if ($dvs == "ALL") {
            $query  = "\nSELECT  B.order_detail_dvs_num ";
        }
        $query .= "\n  FROM  order_common as A ";
        $query .= "\n  LEFT JOIN order_detail B on A.order_common_seqno = B.order_common_seqno ";
        $query .= "\n  LEFT JOIN order_dlvr C on B.order_common_seqno = C.order_common_seqno and C.tsrs_dvs = '수신' ";
        $query .= "\n  LEFT JOIN member D on A.member_seqno = D.member_seqno ";
        $query .= "\n  LEFT JOIN order_opt_history E on B.order_common_seqno = E.order_common_seqno AND E.opt_name = '당일판' ";
        // 재단완료 >=
        // 입고완료 <=
        $query .= "\n  where 1=1 and A.order_state >= '2220' AND A.order_state < '3120'";
        //후공정 유무여부
        if ($this->blankParameterCheck($param ,"after_yn")) {
            $val = substr($param["after_yn"], 1, -1);
            $query .= "\n   AND  B.after_use_yn= '" . $val . "' ";
        }

        //당일판 여부
        if ($this->blankParameterCheck($param ,"theday_yn")) {
                $val = substr($param["theday_yn"], 1, -1);
                if($val == "Y") {
                    $query .= "\n   AND E.opt_name = \"당일판\" ";
                } else if($val == "N") {
                    $query .= "\n   AND E.opt_name IS NULL ";
                }
        }

        //키워드
        if ($this->blankParameterCheck($param ,"office_nick")) {
            $val = substr($param["office_nick"], 1, -1);
            $query .= "\n   AND  D.member_name LIKE '%" . $val . "%' ";
        }
        //발주일
        if ($this->blankParameterCheck($param ,"from")) {
            $val = substr($param["search_cnd"], 1, -1);
            $query .= "\n   AND  A.order_regi_date > " . $param["from"];
        }
        if ($this->blankParameterCheck($param ,"to")) {
            $val = substr($param["search_cnd"], 1, -1);
            $query .= "\n   AND  A.order_regi_date <= " . $param["to"];
        }

        // 배송방법
        if ($this->blankParameterCheck($param ,"dlvr_way")) {
            $dlvr_way = substr($param["dlvr_way"], 1, -1);
            // 성수동
            if($dlvr_way == "07") {
                $query .= "\n   AND  C.dlvr_way IN ('01','02','03','04','05','07')";
            }
            else if($dlvr_way == "06")
                $query .= "\n   AND  C.dlvr_way IN ('06')";
        }

        // 배송방법_상세
        if ($this->blankParameterCheck($param ,"dlvr_way_detail")) {
            $dlvr_way_detail = substr($param["dlvr_way_detail"], 1, -1);
            if($dlvr_way_detail == "1") {
                $query .= "\n   AND  C.dlvr_way IN ('06','07')";
            }
            else if($dlvr_way_detail == "2")
                $query .= "\n   AND  C.dlvr_way IN ('04','05')";
            else if($dlvr_way_detail == "3")
                $query .= "\n   AND  C.dlvr_way IN ('01')";
        }

        // 상품종류
        if ($this->blankParameterCheck($param ,"category")) {
            if($param["category"] == "명함/스티커") {

            }

            $query .= "\n   AND  A.cate_sortcode like ( ". $param["catecode"] . ")";
        }

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        $query .= "\nORDER BY A.depo_finish_date asc";

        if ($dvs == "SEQ") {
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }
        return $conn->Execute($query);
    }

    function selectOrderAfterInfo($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT group_concat(CONCAT(after_name, ' - ' ,detail) SEPARATOR ' / ') as after_detail ";
        $query .= "\n       FROM order_after_history  ";
        $query .= "\n       WHERE order_common_seqno = %s  ";
        $query .= "\n       AND (basic_yn='N' OR after_name = '접지') ";
        $query .= "\n       GROUP BY order_common_seqno ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result->fields["after_detail"];
    }

    function selectOrderAfterInfoForParsel($conn, $param) {

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT group_concat(DISTINCT(after_name) SEPARATOR ',') as after_detail ";
        $query .= "\n       FROM order_after_history  ";
        $query .= "\n       WHERE order_common_seqno = %s  ";
        $query .= "\n       AND (basic_yn='N' OR after_name = '접지') AND after_name != '후지반칼' ";
        $query .= "\n       GROUP BY order_common_seqno ";

        $query  = sprintf($query, $param["order_common_seqno"]);
        $result = $conn->Execute($query);

        return $result->fields["after_detail"];
    }

    function selectReleaseList($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        if ($dvs == "COUNT") {
            $query  = "\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT  A.order_common_seqno, A.title, A.order_state, D.dlvr_way, A.order_regi_date, A.receipt_finish_date, B.dlvr_add_info as invo_cpn, F.cut_size_wid, F.cut_size_vert,B.member_name, C.state_modi_date, A.order_num, A.order_detail, B.addr, B.addr_detail, A.count, A.amt, C.state_modi_date, E.typset_num, D.dlvr_req ";
        }
        $query .= "\n  FROM order_common AS A ";
        $query .= "\n  INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n  INNER JOIN order_state_history AS C ON A.order_common_seqno = C.order_common_seqno AND C.state IN ('2220') ";
        $query .= "\n  INNER JOIN order_dlvr AS D ON A.order_common_seqno = D.order_common_seqno AND D.tsrs_dvs = '수신' ";
        $query .= "\n  LEFT JOIN order_typset AS E ON A.order_num = E.order_num ";
        $query .= "\n  INNER JOIN order_detail AS F ON A.order_common_seqno = F.order_common_seqno ";
        $query .= "\n  LEFT JOIN order_opt_history AS G ON A.order_common_seqno = G.order_common_seqno AND G.opt_name = '당일판'";
        $query .= "\n WHERE 1 = 1 ";

        //$query .= "\n   AND  A.order_common_seqno = D.order_common_seqno ";

        if ($this->blankParameterCheck($param ,"dlvr_dvs")) {
            $val = substr($param["dlvr_dvs"], 1, -1);

            if($val == "outsource") {
                $query .= "\n   AND (A.cate_sortcode IN ('002006001','002006002','008002001','008002002','008002003','006002001','006002002','006002003','006002004','006002005','006002006','006002007','006002008','006002009','006002010','007001001','007001002','007001003','007002001','009001001','009001002','002006001') OR (A.cate_sortcode = '003002001' AND F.cate_paper_mpcode IN (1004, 1005,1006,1007,1008,1009,1010)) OR (A.cate_sortcode = '003003001' AND F.cate_paper_mpcode != 198))";
            } else {
                $query .= "\n   AND  A.dlvr_dvs = '" . $val . "' ";
            }
        }

        if ($this->blankParameterCheck($param ,"member_name")) {
            $val = substr($param["member_name"], 1, -1);
            $query .= "\n   AND  B.member_name LIKE '%" . $val . "%' ";
        }

        if ($this->blankParameterCheck($param ,"theday_yn")) {
            $val = substr($param["theday_yn"], 1, -1);
            if($val == "Y") {
                $query .= "\n    AND  G.opt_name = '당일판' ";
            }
        }

        if ($this->blankParameterCheck($param ,"title")) {
            $val = substr($param["title"], 1, -1);
            $query .= "\n   AND  A.title LIKE '%" . $val . "%' ";
        }

        //D.dlvr_way = '02'
        if ($this->blankParameterCheck($param ,"dlvr_way")) {
            $val = substr($param["dlvr_way"], 1, -1);
            $query .= "\n   AND  D.dlvr_way = '" . $val . "' ";
            if($val == "02") {
                if ($this->blankParameterCheck($param ,"dlvr_way_detail")) {
                    if($param["dlvr_way_detail"] != "'전체'")
                    $query .= "\n   AND  B.dlvr_add_info = " . $param["dlvr_way_detail"];
                }
            } else {
                if ($this->blankParameterCheck($param ,"dlvr_way_detail")) {
                    if($param["dlvr_way_detail"] != "'전체'")
                        $query .= "\n   AND  B.default_release = " . $param["dlvr_way_detail"];
                }
            }

        }

        if ($this->blankParameterCheck($param ,"from")) {
            $val = substr($param["from"], 1, -1);
            $query .= "\n   AND  C.state_modi_date >= '" . $val . " 00:00:00' ";
        }

        if ($this->blankParameterCheck($param ,"to")) {
            $val = substr($param["to"], 1, -1);
            $query .= "\n   AND  C.state_modi_date <= '" . $val . " 23:59:59' ";
        }

        if ($this->blankParameterCheck($param ,"state")) {
            $query .= "\n   AND  A.order_state = " . $param["state"];
        } else {
            $query .= "\n   AND  A.order_state >= '2120' ";
        }

        $query .= "\nGROUP BY A.order_num ";
        $query .= "\nORDER BY B.member_name ASC, C.state_modi_date ASC ";

        if($param["state"] == "'3380'") {
            $query .= "\nLIMIT 30 ";
        }
        return $conn->Execute($query);
    }

    function selectPackageCount($conn, $bun_dlvr_order_num, $order_common_seqno, $dlvr_dvs, $member_seqno) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "SELECT COUNT(*) cnt ";
        $query .= "\n FROM order_dlvr AS A ";
        $query .= "\n INNER JOIN order_common AS B ON A.order_common_seqno = B.order_common_seqno";
        $query .= "\n WHERE A.bun_dlvr_order_num = '%s' AND A.tsrs_dvs = '수신'";
        if($dlvr_dvs == "leaflet") {
            $query .= "\n   AND A.order_common_seqno = " . $order_common_seqno;
        }
        $query .= "\n GROUP BY A.bun_dlvr_order_num";

        $query = sprintf($query, $bun_dlvr_order_num);

        $rs = $conn->Execute($query);
        return $rs->fields["cnt"];
    }

    function selectPackageCountNc($conn, $bun_dlvr_order_num, $order_common_seqno, $dlvr_dvs, $member_seqno) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "SELECT COUNT(*) cnt ";
        $query .= "\n FROM order_dlvr AS A ";
        $query .= "\n INNER JOIN order_common AS B ON A.order_common_seqno = B.order_common_seqno";
        $query .= "\n WHERE A.bun_dlvr_order_num = '%s' AND A.tsrs_dvs = '수신'";
        if($dlvr_dvs == "leaflet") {
            $query .= "\n   AND A.order_common_seqno = " . $order_common_seqno;
        }
        $query .= "\n GROUP BY A.bun_dlvr_order_num";

        $query = sprintf($query, $bun_dlvr_order_num);

        $rs = $conn->Execute($query);
        return $rs->fields["cnt"];
    }

    function selectOrdererInfo($conn, $order_common_seqno) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "SELECT addr, addr_detail, zipcode, tel_num, name ";
        $query .= "\n FROM order_dlvr AS A ";
        $query .= "\n WHERE A.order_common_seqno = %s AND A.tsrs_dvs = '송신'";

        $query = sprintf($query, $order_common_seqno);

        return $conn->Execute($query);
    }

    function selectDeliveryList($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        if ($dvs == "COUNT") {
            $query  = "\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT COUNT(*) cnt, A.order_common_seqno, GROUP_CONCAT(A.order_common_seqno SEPARATOR ',') seqnos, GROUP_CONCAT(A.title SEPARATOR '+ ') title, A.order_state, D.dlvr_way, D.dlvr_sum_way, MAX(dlvr_price) dlvr_price, SUM(A.expec_weight) expec_weight, GROUP_CONCAT(CONCAT(FORMAT(A.amt ,0),A.amt_unit_dvs, ' x ', A.count, '건') SEPARATOR '+') memo ";
            $query .= "\n , A.order_regi_date, D.invo_cpn, D.invo_num, GROUP_CONCAT(CONCAT(F.cut_size_wid, 'x', F.cut_size_vert) SEPARATOR '+') cut_size ,B.member_name, D.recei d_recei, D.zipcode d_zipcode, D.tel_num d_tel_num, D.cell_num d_cell_num, D.addr d_addr, D.addr_detail d_addr_detail, D.dlvr_req d_dlvr_req ";
            $query .= "\n , A.order_num, GROUP_CONCAT(A.order_detail SEPARATOR '+') order_detail , B.addr, B.addr_detail, A.count, A.amt, A.dlvr_dvs";
            $query .= "\n , D.bun_dlvr_order_num, B.member_seqno, GROUP_CONCAT(A.cate_sortcode SEPARATOR '+') cate_sortcode, F.stan_name, F.cate_paper_mpcode ";
        }
        $query .= "\n  FROM order_common AS A ";
        $query .= "\n  INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n  INNER JOIN (SELECT order_common_seqno, order_state_history_seqno, state_modi_date, state FROM order_state_history WHERE state = '2120' GROUP BY order_common_seqno) AS C ON A.order_common_seqno = C.order_common_seqno AND C.state = '2120' ";
        $query .= "\n  INNER JOIN order_dlvr AS D ON A.order_common_seqno = D.order_common_seqno AND D.tsrs_dvs = '수신' ";
        $query .= "\n  INNER JOIN order_detail AS F ON A.order_common_seqno = F.order_common_seqno ";
        $query .= "\n  LEFT JOIN rework_list AS G ON G.rework_order_common_seqno = A.order_common_seqno ";
        $query .= "\n  LEFT JOIN order_after_history AS H ON H.order_common_seqno = A.order_common_seqno AND H.after_name IN ('박','형압','미싱')";
        $query .= "\n  LEFT JOIN order_opt_history I on A.order_common_seqno = I.order_common_seqno AND I.opt_name = '당일판' ";
        $query .= "\n WHERE 1 = 1 AND A.OPI_Date IS NULL AND G.order_common_seqno is null  ";
        //$query .= "\n WHERE 1 = 1 AND A.OPI_Date IS NULL AND G.order_common_seqno is null ";

        //$query .= "\n   AND  A.order_common_seqno = D.order_common_seqno ";

        if ($this->blankParameterCheck($param ,"dlvr_dvs")) {
            $val = substr($param["dlvr_dvs"], 1, -1);
            if($val == "outsource_namecard") {
                $query .= "\n   AND ((A.cate_sortcode IN ('003001001','003002001') AND H.after_name IN ('박','형압','미싱')) OR (A.cate_sortcode = '003003001' AND F.cate_paper_mpcode != 198) ";
                $query .= "\n   OR A.cate_sortcode IN ('003007002','003007003') ";
                $query .= "\n   OR (A.cate_sortcode = '003002001' AND F.cate_paper_mpcode IN (1004,1005,1006,1007,1008,1009,1010))  ";
                $query .= "\n   ) ";
            } else if($val == "outsource_leaflet") {
                $query .= "\n   AND ( ";
                $query .= "\n   A.cate_sortcode IN ('008002001','008002002','008002003','006002001','006002002','006002003','006002004','006002005','006002006','006002007','006002008','006002009','007001001','007001002','007001003','007002001','009001001','009001002') ";
                $query .= "\n   ) ";
            }
            else if($val == "leaflet") {
                $query .= "\n   AND  A.dlvr_dvs = '" . $val . "' ";
                $query .= "\n   AND (";
                $query .= "\n   A.cate_sortcode NOT IN ('003001001', '003002001', '003003001', '003007002','003007003','008002001','008002002','008002003','006002001','006002002','006002003','006002004','006002005','006002006','006002007','006002008','006002009','007001001','007001002','007001003','007002001','009001001','009001002') ";
                $query .= "\n   ) ";
            }
            else {
                $query .= "\n   AND  A.dlvr_dvs = '" . $val . "' ";
                $query .= "\n   AND ((A.cate_sortcode = '003003001' AND F.cate_paper_mpcode = 198) ";
                $query .= "\n   OR A.cate_sortcode NOT IN ('003002001', '003003001', '003007002','003007003','008002001','008002002','008002003','006002001','006002002','006002003','006002004','006002005','006002006','006002007','006002008','006002009','007001001','007001002','007001003','007002001','009001001','009001002') ";
                $query .= "\n   OR (A.cate_sortcode = '003002001' AND F.cate_paper_mpcode NOT IN (1004,1005,1006,1007,1008,1009,1010))  ";
                $query .= "\n   ) AND H.after_name IS NULL ";
            }
        }

        if ($this->blankParameterCheck($param ,"theday_yn")) {
            $val = substr($param["theday_yn"], 1, -1);
            if($val == "Y") {
                $query .= "\n   AND I.opt_name = \"당일판\" ";
            } else if($val == "N") {
                $query .= "\n   AND I.opt_name IS NULL ";
            }
        }

        if ($this->blankParameterCheck($param ,"member_name")) {
            $val = substr($param["member_name"], 1, -1);
            $query .= "\n   AND  B.member_name LIKE '%" . $val . "%' ";
        }

        if ($this->blankParameterCheck($param ,"title")) {
            $val = substr($param["title"], 1, -1);
            $query .= "\n   AND  A.title LIKE '%" . $val . "%' ";
        }

        //D.dlvr_way = '02'
        if ($this->blankParameterCheck($param ,"dlvr_way")) {
            $val = substr($param["dlvr_way"], 1, -1);
            $query .= "\n   AND  D.dlvr_way = '" . $val . "' ";
            if($val == "02") {
                if ($this->blankParameterCheck($param ,"dlvr_way_detail")) {
                    if($param["dlvr_way_detail"] != "'전체'")
                        $query .= "\n   AND  D.invo_cpn = " . $param["dlvr_way_detail"];
                }
            }

        }

        if ($this->blankParameterCheck($param ,"from")) {
            $val = substr($param["from"], 1, -1);
            $query .= "\n   AND  C.state_modi_date >= '" . $val . " 00:00:00' ";
        }

        if ($this->blankParameterCheck($param ,"to")) {
            $val = substr($param["to"], 1, -1);
            $query .= "\n   AND  C.state_modi_date <= '" . $val . " 23:59:59' ";
        }

        if ($this->blankParameterCheck($param ,"state")) {
            $val = substr($param["state"], 1, -1);
            if($val == "Y") {
                $query .= "\n   AND  D.invo_num != '' ";
            } else if($val == "N") {
                $query .= "\n   AND  D.invo_num IS NULL ";
            }
        } else {

        }

        $query .= "\n   AND  A.order_state >= '2120' ";

        $query .= "\nGROUP BY D.bun_dlvr_order_num ";
        $query .= "\nORDER BY B.member_name ASC ";
        //echo $query;exit;
        return $conn->Execute($query);
    }


    function selectDeliveryList2($conn, $param) {
        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }
        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);
        $dvs = substr($param["dvs"], 1, -1);

        if ($dvs == "COUNT") {
            $query  = "\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT COUNT(*) cnt, A.order_common_seqno, GROUP_CONCAT(A.order_common_seqno SEPARATOR ',') seqnos, GROUP_CONCAT(A.title SEPARATOR '+ ') title, A.order_state, D.dlvr_way, D.dlvr_sum_way, MAX(dlvr_price) dlvr_price, SUM(A.expec_weight) expec_weight, GROUP_CONCAT(CONCAT(FORMAT(A.amt ,0),A.amt_unit_dvs, ' x ', A.count, '건') SEPARATOR '+') memo ";
            $query .= "\n , A.order_regi_date, D.invo_cpn, D.invo_num, GROUP_CONCAT(CONCAT(F.cut_size_wid, 'x', F.cut_size_vert) SEPARATOR '+') cut_size ,B.member_name, D.recei d_recei, D.zipcode d_zipcode, D.tel_num d_tel_num, D.cell_num d_cell_num, D.addr d_addr, D.addr_detail d_addr_detail, D.dlvr_req d_dlvr_req ";
            $query .= "\n , A.order_num, GROUP_CONCAT(A.order_detail SEPARATOR '+') order_detail , B.addr, B.addr_detail, A.count, A.amt, A.dlvr_dvs";
            $query .= "\n , D.bun_dlvr_order_num, B.member_seqno, GROUP_CONCAT(A.cate_sortcode SEPARATOR '+') cate_sortcode, F.stan_name, F.cate_paper_mpcode ";
        }
        $query .= "\n  FROM order_common AS A ";
        $query .= "\n  INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n  INNER JOIN (SELECT order_common_seqno, order_state_history_seqno, state_modi_date, state FROM order_state_history WHERE state = '2120' GROUP BY order_common_seqno) AS C ON A.order_common_seqno = C.order_common_seqno AND C.state = '2120' ";
        $query .= "\n  INNER JOIN order_dlvr AS D ON A.order_common_seqno = D.order_common_seqno AND D.tsrs_dvs = '수신' ";
        $query .= "\n  INNER JOIN order_detail AS F ON A.order_common_seqno = F.order_common_seqno ";
        $query .= "\n  LEFT JOIN rework_list AS G ON G.rework_order_common_seqno = A.order_common_seqno ";
        $query .= "\n  LEFT JOIN order_after_history AS H ON H.order_common_seqno = A.order_common_seqno AND H.after_name IN ('박','형압','미싱')";
        $query .= "\n  LEFT JOIN order_opt_history I on A.order_common_seqno = I.order_common_seqno AND I.opt_name = '당일판' ";
        $query .= "\n WHERE 1 = 1 AND A.OPI_Date IS NULL AND G.order_common_seqno is null  ";
        //$query .= "\n WHERE 1 = 1 AND A.OPI_Date IS NULL AND G.order_common_seqno is null ";

        //$query .= "\n   AND  A.order_common_seqno = D.order_common_seqno ";

        if ($this->blankParameterCheck($param ,"dlvr_dvs")) {
            $val = substr($param["dlvr_dvs"], 1, -1);
            if($val == "outsource_namecard") {
                $query .= "\n   AND ((A.cate_sortcode IN ('003001001','003002001') AND H.after_name IN ('박','형압','미싱')) OR (A.cate_sortcode = '003003001' AND F.cate_paper_mpcode != 198) ";
                $query .= "\n   OR A.cate_sortcode IN ('003007002','003007003') ";
                $query .= "\n   OR (A.cate_sortcode = '003002001' AND F.cate_paper_mpcode IN (1004,1005,1006,1007,1008,1009,1010))  ";
                $query .= "\n   ) ";
            } else if($val == "outsource_leaflet") {
                $query .= "\n   AND ( ";
                $query .= "\n   A.cate_sortcode IN ('008002001','008002002','008002003','006002001','006002002','006002003','006002004','006002005','006002006','006002007','006002008','006002009','007001001','007001002','007001003','007002001','009001001','009001002') ";
                $query .= "\n   ) ";
            }
            else if($val == "leaflet") {
                $query .= "\n   AND  A.dlvr_dvs = '" . $val . "' ";
                $query .= "\n   AND (";
                $query .= "\n   A.cate_sortcode NOT IN ('003001001', '003002001', '003003001', '003007002','003007003','008002001','008002002','008002003','006002001','006002002','006002003','006002004','006002005','006002006','006002007','006002008','006002009','007001001','007001002','007001003','007002001','009001001','009001002') ";
                $query .= "\n   ) ";
            }
            else {
                $query .= "\n   AND  A.dlvr_dvs = '" . $val . "' ";
                $query .= "\n   AND ((A.cate_sortcode = '003003001' AND F.cate_paper_mpcode = 198) ";
                $query .= "\n   OR A.cate_sortcode NOT IN ('003002001', '003003001', '003007002','003007003','008002001','008002002','008002003','006002001','006002002','006002003','006002004','006002005','006002006','006002007','006002008','006002009','007001001','007001002','007001003','007002001','009001001','009001002') ";
                $query .= "\n   OR (A.cate_sortcode = '003002001' AND F.cate_paper_mpcode NOT IN (1004,1005,1006,1007,1008,1009,1010))  ";
                $query .= "\n   ) AND H.after_name IS NULL ";
            }
        }

        if ($this->blankParameterCheck($param ,"theday_yn")) {
            $val = substr($param["theday_yn"], 1, -1);
            if($val == "Y") {
                $query .= "\n   AND I.opt_name = \"당일판\" ";
            } else if($val == "N") {
                $query .= "\n   AND I.opt_name IS NULL ";
            }
        }

        if ($this->blankParameterCheck($param ,"member_name")) {
            $val = substr($param["member_name"], 1, -1);
            $query .= "\n   AND  B.member_name LIKE '%" . $val . "%' ";
        }

        if ($this->blankParameterCheck($param ,"title")) {
            $val = substr($param["title"], 1, -1);
            $query .= "\n   AND  A.title LIKE '%" . $val . "%' ";
        }

        //D.dlvr_way = '02'
        if ($this->blankParameterCheck($param ,"dlvr_way")) {
            $val = substr($param["dlvr_way"], 1, -1);
            $query .= "\n   AND  D.dlvr_way = '" . $val . "' ";
            if($val == "02") {
                if ($this->blankParameterCheck($param ,"dlvr_way_detail")) {
                    if($param["dlvr_way_detail"] != "'전체'")
                        $query .= "\n   AND  D.invo_cpn = " . $param["dlvr_way_detail"];
                }
            }

        }

        if ($this->blankParameterCheck($param ,"from")) {
            $val = substr($param["from"], 1, -1);
            $query .= "\n   AND  C.state_modi_date >= '" . $val . " 00:00:00' ";
        }

        if ($this->blankParameterCheck($param ,"to")) {
            $val = substr($param["to"], 1, -1);
            $query .= "\n   AND  C.state_modi_date <= '" . $val . " 23:59:59' ";
        }

        if ($this->blankParameterCheck($param ,"state")) {
            $val = substr($param["state"], 1, -1);
            if($val == "Y") {
                $query .= "\n   AND  D.invo_num != '' ";
            } else if($val == "N") {
                $query .= "\n   AND  D.invo_num IS NULL ";
            }
        } else {

        }

        $query .= "\n   AND  A.order_state >= '2120' ";

        $query .= "\nGROUP BY D.bun_dlvr_order_num ";
        $query .= "\nORDER BY B.member_name ASC ";
        echo $query;
       // return $conn->Execute($query);
    }

    /**
     * @brief 접수리스트
     *
     * @param $conn  = connection identifier
     * @param $param = 검색조건 파라미터
     *
     * @return 검색결과
     */
    function selectPaperMaterialsMngList($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $dvs = substr($param["dvs"], 1, -1);

        if ($dvs == "COUNT") {
            $query  = "\nSELECT  COUNT(*) AS cnt ";
        } else if ($dvs == "SEQ") {
            $query  = "\nSELECT  paper_op_seqno ";
            $query .= "\n       ,typset_num ";
            $query .= "\n       ,op_date ";
            $query .= "\n       ,name ";
            $query .= "\n       ,dvs ";
            $query .= "\n       ,color ";
            $query .= "\n       ,basisweight ";
            $query .= "\n       ,op_affil ";
            $query .= "\n       ,op_size ";
            $query .= "\n       ,stor_subpaper ";
            $query .= "\n       ,stor_size ";
            $query .= "\n       ,grain ";
            $query .= "\n       ,amt ";
            $query .= "\n       ,amt_unit ";
            $query .= "\n       ,memo ";
            $query .= "\n       ,typ ";
            $query .= "\n       ,typ_detail ";
            $query .= "\n       ,orderer ";
            $query .= "\n       ,flattyp_dvs ";
            $query .= "\n       ,state ";
            $query .= "\n       ,regi_date ";
            $query .= "\n       ,extnl_brand_seqno ";
            $query .= "\n       ,storplace ";
        }
        $query .= "\n   FROM  paper_op ";
        $query .= "\n   WHERE  1=1 ";

        //주문일 접수일
        if ($this->blankParameterCheck($param ,"from")) {
            $val = substr($param["search_cnd1"], 1, -1);
            $query .= "\n   AND  " . $val ." >= " . $param["from"];
        }
        if ($this->blankParameterCheck($param ,"to")) {
            $val = substr($param["search_cnd1"], 1, -1);
            $query .= "\n   AND  " . $val. " <= " . $param["to"];
        }

        //발주상태
        if ($this->blankParameterCheck($param ,"state")) {
            $query .= "\n   AND  state = " . $param["state"];
        }

        //발주번호
        if ($this->blankParameterCheck($param ,"paper_op_seqno")) {
            $query .= "\n   AND  paper_op_seqno = " . $param["paper_op_seqno"];
        }

        //수주처
        if ($this->blankParameterCheck($param ,"extnl_brand_seqno")) {
            $query .= "\n   AND  extnl_brand_seqno = " . $param["extnl_brand_seqno"];
        }

        //입고처
        if ($this->blankParameterCheck($param ,"storplace")) {
            $query .= "\n   AND  storplace = " . $param["storplace"];
        }

        $query .= "\n   ORDER BY paper_op_seqno DESC";

        $s_num = substr($param["s_num"], 1, -1);
        $list_num = substr($param["list_num"], 1, -1);

        if ($dvs == "SEQ") {
            $query .= "\nLIMIT ". $s_num . ", " . $list_num;
        }

        return $conn->Execute($query);
    }

    /**
     * @brief 종이발주 취소
     *
     * @param $conn  = connection identifier
     * @param $param = 조건 파라미터
     *
     * @return 검색결과
     */
    function updatePaperMaterialsMngCancel($conn, $param) {

        //커넥션 체크
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        //인젝션 어택 방지
        $param = $this->parameterArrayEscape($conn, $param);

        $query .= "\nUPDATE  paper_op ";
        $query .= "\n   SET  state = '530' ";
        $query .= "\n WHERE  paper_op_seqno = %s ";

        $query = sprintf($query, $param["paper_op_seqno"]);

        $resultSet = $conn->Execute($query);

        if ($resultSet === FALSE) {
            return false;
        } else {
            return true;
        }
    }
}
?>
