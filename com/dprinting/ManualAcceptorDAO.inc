<?php
include_once(INC_PATH . '/com/nexmotion/job/nimda/common/NimdaCommonDAO.inc');
include_once(INC_PATH . "/common_lib/cypress_file.inc");
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-05-26
 * Time: 오후 5:36
 */
$curDirectory = dirname(__FILE__);
define("LOGPATH", $curDirectory . "/logs/" . date("Y_m_d"));

class ManualAcceptorDAO extends NimdaCommonDAO {

    function __construct() {
    }

    public function checkIDAndPassword($conn, $param) {
        $id = $param['id'];
        $pw = $param['pw'];

        $query = "SELECT * /* 수동접수 - 로그인정보 check */";
        $query .= "FROM empl ";
        $query .= "WHERE empl_id = '%s' AND passwd = PASSWORD('%s') LIMIT 1 ";

        $query  = sprintf($query, $id, $pw);

        $rs = $conn->Execute($query);

        if ($rs && !$rs->EOF) {
            return $rs->fields['empl_id'];
        } else {
            return false;
        }
    }

    public function checkIDAndToken($conn, $param) {
        $token = $param['token'];

        $query = "SELECT * /* 수동접수 - 로그인정보 check */";
        $query .= "FROM empl_login_rcd ";
        $query .= "WHERE token = %s LIMIT 1 ";

        $query  = sprintf($query, $token);
        $rs = $conn->Execute($query);

        if ($rs && !$rs->EOF) {
            return $rs->fields['empl_id'];
        } else {
            return false;
        }
    }

    public function insertLoginRecord($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query =  " INSERT /* 수동접수 - 로그인기록 insert */ ";
        $query .= " INTO empl_login_rcd ";
        $query .= " (empl_id, event, token, access_ip, oper_sys) VALUES ";
        $query .= " (%s, %s, %s, %s, %s) ";

        $query = sprintf($query
            , $param['empl_id']
            , $param['kind']
            , $param['token']
            , $param['access_ip']
            , $param['oper_sys']);

        return $conn->Execute($query);
    }

    public function isValidLogin($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = " SELECT * /* 수동접수 - 로그인정보 얻기 */";
        $query .= " FROM empl_login_rcd ";
        $query .= " WHERE token = %s and event = 'logout' ";

        $query = sprintf($query, $param['token']);
        $rs = $conn->Execute($query);

        while ($rs && !$rs->EOF) {
            return false;
        }

        return true;
    }

    public function selectAutoStatisticsForToday($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param['to_date'] = $param['to_date'] . " 23:59:59";
        $param = $this->parameterArrayEscape($conn, $param);

        $result = array();

        $query = "\n    SELECT ifnull(count(*),0) as full_count_by_order ";
        $query .= "\n   , ifnull(sum(count),0) as full_count_by_item ";
        $query .= "\n    FROM order_common ";
        $query .= "\n    WHERE receipt_start_date BETWEEN %s and %s ";
        $query .= "\n    AND order_state >= '1335' ";
        $query = sprintf($query, $param['from_date'], $param['to_date']);

        $rs1 = $conn->execute($query);

        $query = "\n    SELECT ifnull(count(*),0) as pass_count_by_order ";
        $query .= "\n   ,ifnull(sum(count),0) as pass_count_by_item ";
        $query .= "\n   FROM order_common ";
        $query .= "\n    WHERE receipt_start_date BETWEEN %s and %s ";
        $query .= "\n    AND order_state >= '1335' ";
        $query .= "\n   AND receipt_dvs = 'Auto' ";
        $query = sprintf($query, $param['from_date'], $param['to_date']);

        $rs2 = $conn->execute($query);

        $result['full_count_by_order'] = $rs1->fields['full_count_by_order'];
        $result['full_count_by_item'] = $rs1->fields['full_count_by_item'];
        $result['pass_count_by_order'] = $rs2->fields['pass_count_by_order'];
        $result['pass_count_by_item'] = $rs2->fields['pass_count_by_item'];

        return $result;
    }

    public function updateAutoSet($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    UPDATE /* 업데이트 */ auto_set ";
        $query .= "\n   SET exec_yn = %s ";
        $query .= "\n   LIMIT 1";

        $query = sprintf($query, $param['command']);

        if($conn->Execute($query)) {
            return true;
        } else {
            return false;
        }
    }

    public function selectUserinfoFromToken($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = " SELECT * /* 수동접수 - 로그인정보 얻기 */";
        $query .= " FROM empl_login_rcd ";
        $query .= " WHERE token = %s ";

        $query = sprintf($query, $param['token']);

        $rs = $conn->Execute($query);


        while ($rs && !$rs->EOF) {
            $result['empl_id'] = $rs->fields['empl_id'];
            $result['access_ip'] = $rs->fields['access_ip'];
            $result['oper_sys'] = $rs->fields['oper_sys'];
            return $result;
        }

        return null;
    }

    public function selectAcceptorInfo($conn, $param) {
        $id = $param['empl_id'];

        $query = "SELECT * /* 수동접수 - 접수자정보 얻기 */";
        $query .= "FROM empl ";
        $query .= "WHERE empl_id = '%s' ";

        $query  = sprintf($query, $id);

        return $conn->Execute($query);
    }

    public function selectOrderDetailFileNumFromTypsetNum($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    select * from sheet_typset AS A ";
        $query .= "\n  INNER JOIN amt_order_detail_sheet AS B ON A.sheet_typset_seqno = B.sheet_typset_seqno ";
        $query .= "\n  INNER JOIN order_detail_count_file AS C ON C.order_detail_count_file_seqno = B.order_detail_count_file_seqno ";
        $query .= "\n  where A.typset_num = %s ";

        $query = sprintf($query, $param['plate_id']);

        return $conn->Execute($query);
    }

    public function cancelPlate($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    UPDATE sheet_typset AS A";
        $query .= "\n   INNER JOIN amt_order_detail_sheet AS B ON A.sheet_typset_seqno = B.sheet_typset_seqno ";
        $query .= "\n   SET A.state = '1180', B.sheet_typset_seqno = NULL, B.state = '2120' ";
        $query .= "\n   WHERE A.typset_num = %s ";

        $query = sprintf($query, $param['plate_id']);

        $conn->Execute($query);
    }

    public function updateWorkByCount($conn, $param) {
        $param['order_num'] = substr($param['order_detail_file_num'], 1, strlen($param['order_detail_file_num']) - 5);

        if(strlen($param['order_detail_file_num']) != 21) {
            return false;
        }

        CLS_File::FileWrite(LOGPATH, "request_method : " . $param['request_method'],"a+");
        // order_detail_count_file의 건별 업데이트
        $query = "\n  UPDATE order_detail_count_file ";
        $query .= "\n   SET state = '%s' ";

        $query .= "\n   WHERE  order_detail_file_num = '%s' ";

        $query = sprintf($query, $param['status'], $param['order_detail_file_num']);

        CLS_File::FileWrite(LOGPATH, "query : " . $query ,"a+");
        if(!$conn->Execute($query))
            return false;

        $param['before_state'] = $this->selectOrderDetailFileNumState($conn,$param);
        $state_change_query = "\n INSERT INTO state_change_admin ";
        $state_change_query .= "\n (order_detail_file_num, before_state, aft_state) ";
        $state_change_query .= "\n VALUES";
        $state_change_query .= "\n (%s, %s, %s) ";
        $state_change_query =
            sprintf($state_change_query, $param['order_detail_file_num'], $param['status'], $param['before_state']);
        $conn->Execute($state_change_query);

        if($param['status'] == '1320') {
            $this->UpdateWork($conn,$param);
        } else if($this->isAllCountSameState($conn,$param)) {
            $this->UpdateWork($conn,$param);
        }

        return true;
    }

    public function selectOrderDetailCountFileNums($conn, $sheet_typset_seqno) {
        $query = "\n    select * from order_detail_count_file AS A ";
        $query .= "\n   INNER JOIN amt_order_detail_sheet AS B ON A.order_detail_count_file_seqno = B.order_detail_count_file_seqno ";
        $query .= "\n   WHERE sheet_typset_seqno = %s ";

        $query = sprintf($query, $sheet_typset_seqno);

        return $conn->Execute($query);
    }

    public function selectOneFileOrderNum($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $order_num = $param['order_num'];
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT DISTINCT(order_num) from order_dlvr AS A";
        $query .= "\n   INNER JOIN order_common AS B ON A.order_common_seqno = B.order_common_seqno ";
        $query .= "\n   WHERE bun_dlvr_order_num IN (select bun_dlvr_order_num from order_common AS A ";
        $query .= "\n   INNER JOIN order_dlvr AS B ON A.order_common_seqno = B.order_common_seqno";
        $query .= "\n   WHERE A.order_num = %s and B.tsrs_dvs = '수신')";

        $query = sprintf($query, $param['order_num']);
        $rs = $conn->Execute($query);

        $tied_order_ids = array();
        while ($rs && !$rs->EOF) {
            $tied_order_id = $rs->fields['order_num'];
            if($tied_order_id != $order_num)
                array_push($tied_order_ids, $tied_order_id);
            $rs->MoveNext();
        }

        return $tied_order_ids;
    }

    public function isAllCountSameState($conn, $param) {
        $query = "\n    SELECT state ";
        $query .= "\n   FROM order_detail_count_file ";
        $query .= "\n   WHERE order_detail_file_num like '%" . $param['order_num'] . "%'";

        CLS_File::FileWrite(LOGPATH, "query : " . $query ,"a+");
        $rs = $conn->Execute($query);

        while ($rs && !$rs->EOF) {
            if($rs->fields['state'] != $param['status']) {
                return false;
            }
            $rs->MoveNext();
        }

        return true;
    }

    public function updateWork($conn, $param) {
        $query = " UPDATE /* 수동접수 - 작업수정 */ order_common ";
        $query .= " SET order_state = '" . $param['status'] ."' ";

        if(array_key_exists('order_name',$param)) {
            $query .= " , title = '" . $param['order_name'] . "' ";
        }

        if(array_key_exists('qc_check_pc',$param)) {
            $query .= " , qc_check_pc = '" . $param['qc_check_pc'] . "' ";
        }

        if(array_key_exists('receipt_dvs',$param)) {
            $query .= " , receipt_dvs = '" . $param['receipt_dvs'] . "' ";
        }

        if($param['status'] == "1330") {
            $result_path = SITE_DEFAULT_ORDER_DETAIL_COUNT_FILE . "/"
                . date("Y/m/d", mktime(0,0,0,date("m")  , date("d"), date("Y")));

            $path_result = $this->getPrintfilepath($conn, $param['order_num']);

            $update_param = array();
            $update_param['file_path'] = $result_path;
            $update_param['print_file_path'] = $path_result['print_file_path'];
            $update_param['order_detail_dvs_num'] = $path_result['order_detail_dvs_num'];
            $cnt = $path_result['count'];
            for($i = 1; $i <= $cnt ; $i++) {
                $order_detail_file_num = $update_param['order_detail_dvs_num'] . str_pad($i, 2, "0", STR_PAD_LEFT);
                $update_param['save_file_name'] = $order_detail_file_num . ".pdf";
                $update_param['print_file_name'] = $order_detail_file_num . ".pdf";
                $update_param['order_detail_file_num'] = $order_detail_file_num;
                $this->updateOrderDetailCountFileInfo($conn,$update_param);
            }
            $query .= " , receipt_start_date = now() ";
        }

        if($param['status'] == "1335") {
            $query .= " , receipt_finish_date = now() ";
        }

        if($param['status'] == "1360") {
            $query .= " , receipt_finish_date = now() ";
        }

        if(strpos($param['status'], "1180")) {
            $query .= " , eraser = '" . $param['empl_id'] . "' ";
        }

        $query .= "WHERE order_num = '" . $param['order_num'] . "' ";

        CLS_File::FileWrite(LOGPATH, "query : " . $query ,"a+");
        // order_common 업데이트
        if(!$conn->Execute($query))
            return false;


        $query = " UPDATE /* 수동접수 - 작업수정 */ order_detail ";
        $query .= " SET state = '" . $param['status'] ."' ";

        if(array_key_exists('acceptor_memo',$param)) {
            $query .= " , receipt_memo = '" . $param['acceptor_memo'] . "' ";
        }

        if(array_key_exists('acceptor_id',$param)) {
            $query .= " , receipt_mng = '" . $param['acceptor_id'] . "' ";
        }

        if($param['status'] == "1330") {
            $query .= " , receipt_start_date = now() ";
        }

        if($param['status'] == "1335") {
            $query .= " , receipt_finish_date = now() ";
        }

        $query .= "WHERE order_detail_dvs_num like '%" . $param['order_num'] . "%' ";

        CLS_File::FileWrite(LOGPATH, "query : " . $query ,"a+");
        //order_detail 업데이트
        if(!$conn->Execute($query))
            return false;


        $query = " UPDATE /* 접수 - 주문정보수정 */ order_detail_count_file ";
        $query .= " SET state = '" . $param['status'] ."' ";
        $query .= "WHERE order_detail_file_num like '%" . $param['order_num'] . "%' ";

        //order_detail_count_file 업데이트
        CLS_File::FileWrite(LOGPATH, "query : " . $query ,"a+");
        if(!$conn->Execute($query))
            return false;

        return $param['order_num'];
    }

    public function selectWorkListForAuto($conn, $param) {

    }

    public function getPlateDetailForCommercial($conn, $param) {

    }

    public function getPaperEtprs($conn, $manu_name) {
        $query = "\n    SELECT extnl_etprs_seqno ";
        $query .= "\n    FROM extnl_etprs ";
        $query .= "\n   WHERE pur_prdt = '종이' ";
        $query .= "\n   AND manu_name = '" . $manu_name . "' ";

        $rs = $conn->Execute($query);
        return $rs->fields['extnl_etprs_seqno'];

    }

    public function getPlateListForCommercial($conn, $param) {
        $from_date = "";
        $to_date = "";
        if($param['created_date_range'] !== null) {
            $date = explode('_', $param['created_date_range']);
            $from_date = $date[0];
            $to_date = $date[1];
        }


        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT /* 상업인쇄 판생성 정보 */";
        $query .= "\n   A.typset_num, A.state, B.origin_file_name, A.regi_date, A.sheet_typset_seqno, ";
        $query .= "\n   A.dlvrboard, A.size, A.paper_name, A.paper_color, A.paper_basisweight, A.beforeside_tmpt, A.aftside_tmpt, ";
        $query .= "\n   A.print_amt, A.print_amt_unit, A.print_etprs, A.memo, C.empl_id, C.name, A.save_path,";
        $query .= "\n   (SELECT count(*) FROM amt_order_detail_sheet WHERE sheet_typset_seqno = A.sheet_typset_seqno) AS cnt ";
        $query .= "\n   FROM sheet_typset AS A ";
        $query .= "\n   LEFT JOIN sheet_typset_file AS B ON A.sheet_typset_seqno = B.sheet_typset_seqno";
        $query .= "\n   LEFT JOIN empl AS C ON A.empl_seqno = C.empl_seqno ";
        $query .= "\n   WHERE A.typset_way = 'COMMERCIAL' ";

        if($param['created_date_range'] !== null) {
            $query .= " AND A.regi_date BETWEEN '" . $from_date . "' AND '" . $to_date . " 23:59:59' ";
        }

        $query .= " ORDER BY A.regi_date ";

        if(array_key_exists('typset_num', $param)) {
            $query .= "\n AND A.typset_num = %s ";
            $query = sprintf($query, $param['typset_num']);
        }


        return $conn->Execute($query);
    }

    public function getEmplSeqno($conn, $empl_id) {
        $query = "\n   SELECT empl_seqno ";
        $query .= "\n  FROM empl ";
        $query .= "\n   WHERE empl_id = '%s' ";

        $query = sprintf($query, $empl_id);
        $rs = $conn->Execute($query);

        return $rs->fields['empl_seqno'];
    }

    public function selectTypsetPath($conn, $typset_num) {
        $query = "\n    SELECT * FROM sheet_typset ";
        $query .= "\n   WHERE typset_num = '" . $typset_num . "'";

        return $conn->Execute($query);
    }

    public function makeNewTypsetNum($conn, $param) {
        $date = date('Y-m-d');
        $query = "\n    SELECT count(*) + 1 AS cnt ";
        $query .= "\n   FROM sheet_typset ";
        $query .= "\n   WHERE regi_date BETWEEN '". $date ."' AND '". $date . " 23:59:59' AND typset_num like 'COM%' ";

        $rs = $conn->Execute($query);

        $count = 0;
        while($rs && !$rs->EOF) {
            $count = $rs->fields['cnt'];
            $rs->MoveNext();
        }

        $count = str_pad($count, 3, "0", STR_PAD_LEFT);
        $date = substr(date("Ymd"), 2);
        $typset_num = "COM" . $date . $count . "-01";

        return $typset_num;
    }

    public function selectItemsInPlate($conn, $param) {
        $query = "\n    SELECT /* 판에 있는 주문정보들 */ ";
        $query .= "\n   distinct(C.order_num), C.order_num, C.title, D.cate_sortcode, F.cate_name, C.count, D.side_dvs, D.tot_tmpt, C.amt, E.name, E.color, E.basisweight, G.member_id, G.member_name ";
        $query .= "\n   , D.stan_name, C.cust_memo, A.save_path, D.print_tmpt_name ";
        $query .= "\n   FROM sheet_typset AS A ";
        $query .= "\n   INNER JOIN amt_order_detail_sheet AS B ON A.sheet_typset_seqno = B.sheet_typset_seqno ";
        $query .= "\n   INNER JOIN order_common AS C ON C.order_num = B.order_num ";
        $query .= "\n   INNER JOIN order_detail AS D ON C.order_common_seqno = D.order_common_seqno ";
        $query .= "\n   INNER JOIN cate_paper AS E ON D.cate_paper_mpcode = E.mpcode ";
        $query .= "\n   INNER JOIN cate AS F ON D.cate_sortcode = F.sortcode ";
        $query .= "\n   INNER JOIN member AS G ON G.member_seqno = C.member_seqno ";
        $query .= "\n   WHERE A.sheet_typset_seqno = %s ";

        $query = sprintf($query, $param['sheet_typset_seqno']);
        return $conn->Execute($query);
    }

    public function selectExtnlEtprsSeqno($conn, $manu_name) {
        $query = "\n    SELECT /* 외주업체 키값 가져오기 */ ";
        $query .= "\n    extnl_etprs  ";
        $query .= "\n   WHERE manu_name like '%" . $manu_name . "%'";

        $rs = $conn->Execute($query);
        $extnl_etprs_seqno = "";
        while($rs && !$rs->EOF) {
            $extnl_etprs_seqno = $rs->fields['extnl_etprs_seqno'];
            $rs->MoveNext();
        }

        return $extnl_etprs_seqno;
    }

    public function selectLastInsertedPaperOrder($conn) {
        $query = "\n    SELECT * FROM paper_op order by paper_op_seqno desc limit 1 ";
        return $conn->Execute($query);
    }

    public function updatePaperOrdersSequence($conn, $param) {
        $order_ids = explode('|',$param['order_ids']);

        $update_param = array();
        $update_param["table"] = "paper_op";
        $update_param["prk"] = "paper_op_seqno";

        foreach($order_ids as $order_id) {
            $update_param["col"]["op_degree"] = $param['sequence'];
            $update_param["prkVal"] = $order_id;

            $this->updateData($conn, $param);
        }

        return true;
    }

    public function updatePaperOrder($conn, $param) {
        $masks = array();
        if(array_key_exists('masks',$param))
            $masks = explode('|',$param['masks']);


        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param["table"] = "paper_op";

        if(in_array("PAPER_MILL", $masks))
            $param["col"]["extnl_etprs_seqno"] = $param['paper_mill'];

        if(in_array("PAPER_INFO", $masks)) {
            $param["col"]["name"] = $param['name'];
            $param["col"]["color"] = $param['color'];
            $param["col"]["basisweight"] = $param['basisweight'];
        }

        if(in_array("PAPER_SIZE_1", $masks))
            $param["col"]["op_size"] = $param['paper_size_1'];

        if(in_array("PAPER_SIZE_2", $masks))
            $param["col"]["stor_size"] = $param['paper_size_2'];

        if(in_array("PAPER_GRAIN", $masks))
            $param["col"]["grain"] = $param['paper_grain'];

        if(in_array("QUANTITY", $masks))
            $param["col"]["amt"] = $param['quantity'];

        if(in_array("PRINT_HOUSE", $masks))
            $param["col"]["storplace"] = $param['print_house'];

        if(in_array("MEMO", $masks))
            $param["col"]["memo"] = $param['memo'];

        if(in_array("SEQUENCE", $masks))
            $param["col"]["op_degree"] = $param['sequence'];

        $param["prk"] = "paper_op_seqno";
        $param["prkVal"] = $param['order_id'];

        return $this->updateData($conn, $param);
    }

    public function selectPaperorderList($conn, $param) {
        $date = explode('_', $param['ordered_date_range']);
        $from_date = $date[0];
        $to_date = $date[1];

        $query = "\n    SELECT A.*, B.manu_name ";
        $query .= "\n   FROM paper_op AS A ";
        $query .= "\n   LEFT JOIN extnl_etprs AS B ON A.extnl_etprs_seqno = B.extnl_etprs_seqno ";
        $query .= "\n   WHERE regi_date BETWEEN '" . $from_date . "' AND '" . $to_date . " 23:59:59' ";

        return $conn->Execute($query);
    }

    public function selectWorkListForAllByCount($conn, $param) {
        $date = explode('_', $param['accepted_date_range']);
        $from_date = $date[0];
        $to_date = $date[1];

        $categories = explode('|', $param['categories']);

        if(array_key_exists('masks',$param))
            $masks = explode('|',$param['masks']);

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "SELECT /* 수동접수 - 작업목록 얻기 (건별) */ ";
        $query .= " A.order_detail_file_num, C.title, C.cate_sortcode, F.cate_name, D.member_id,  ";
        $query .= " D.member_name, G.name as paper_name, G.color, G.basisweight, B.stan_name, B.order_detail, ";
        $query .= " B.print_tmpt_name, C.amt, E.empl_id, E.name as empl_name, B.state, ";
        $query .= " A.file_path, A.save_file_name, B.side_dvs, C.receipt_finish_date, B.cut_size_wid, B.cut_size_vert ";
        $query .= " FROM order_detail_count_file AS A ";
        $query .= " INNER JOIN order_detail AS B ON A.order_detail_seqno = B.order_detail_seqno ";
        $query .= " INNER JOIN order_common AS C ON B.order_common_seqno = C.order_common_seqno ";
        $query .= " INNER JOIN member AS D ON C.member_seqno = D.member_seqno ";
        $query .= " LEFT JOIN empl AS E ON E.empl_id = C.receipt_mng ";
        $query .= " INNER JOIN cate AS F ON F.sortcode = C.cate_sortcode ";
        $query .= " INNER JOIN cate_paper AS G ON G.mpcode = B.cate_paper_mpcode ";
        $query .= " WHERE C.receipt_finish_date BETWEEN '%s' AND '%s 23:59:59' ";

        if(count($categories) > 0) {
            $query .= " AND ( 1 = 0 ";
            foreach ($categories as $category) {
                $query .= " OR C.cate_sortcode like '" . $category . "%%' ";
            }
            $query .= " ) ";
        }

        $query .= " ORDER BY C.receipt_finish_date ";

        $query = sprintf($query, $from_date, $to_date);

        CLS_File::FileWrite(LOGPATH, "query : " . $query ,"a+");
        return $conn->Execute($query);
    }

    function updateManualWorking($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $seqno = $this->arr2paramStr($conn, $param['seqno']);
        $param = $this->parameterArrayEscape($conn, $param);


        $query  = "\n UPDATE  order_detail ";
        $query .= "\n   SET  receipt_mng = %s";
        $query .= "\n  WHERE  order_common_seqno IN (%s)";

        $query  = sprintf($query, $param['empl_id'] ,$seqno);

        CLS_File::FileWrite(LOGPATH, "query : " . $query ,"a+");
        return $conn->Execute($query);
    }

    function selectAfterProcesses($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  * ";
        $query .= "\n  FROM  order_after_history ";
        $query .= "\n  WHERE  order_common_seqno = %s";

        $query  = sprintf($query, $param['order_common_seqno']);
        return $conn->Execute($query);
    }

    function selectOptions($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query  = "\n SELECT  * ";
        $query .= "\n  FROM  order_opt_history ";
        $query .= "\n  WHERE  order_common_seqno = %s";

        $query  = sprintf($query, $param['order_common_seqno']);
        CLS_File::FileWrite(LOGPATH, "query : " . $query ,"a+");
        return $conn->Execute($query);
    }

    public function selectWorkDetail($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $masks = array();
        if(array_key_exists('masks',$param))
            $masks = explode('|',$param['masks']);

        $param = $this->parameterArrayEscape($conn, $param);

        $query = " SELECT /* 수동접수 - 주문상세정보 */ ";
        $query .= " A.order_num, A.title, A.order_common_seqno, A.order_state, A.count, A.amt_unit_dvs, A.receipt_finish_date,  ";
        $query .= " A.cate_sortcode, C.cate_name, B.order_detail_dvs_num, B.typset_way, B.order_detail, B.print_tmpt_name, B.page_amt, ";
        $query .= " E.member_id, E.member_name, A.file_upload_dvs, A.owncompany_img_num, ";
        $query .= " A.count, A.order_regi_date, D.name, D.color, D.basisweight, ";
        $query .= " B.work_size_wid, B.work_size_vert, B.cut_size_wid, B.cut_size_vert, ";
        $query .= " B.stan_name, B.side_dvs, B.tot_tmpt, B.amt, A.cust_memo, F.file_path, F.save_file_name, B.receipt_mng, B.receipt_memo, ";
        $query .= " G.dlvr_way, G.dlvr_sum_way, H.name as empl_name ";
        $query .= " FROM order_common AS A ";
        $query .= " INNER JOIN order_detail AS B ON A.order_common_seqno = B.order_common_seqno ";
        $query .= " INNER JOIN cate AS C ON A.cate_sortcode = C.sortcode ";
        $query .= " INNER JOIN cate_paper AS D ON B.cate_paper_mpcode = D.mpcode ";
        $query .= " INNER JOIN member AS E ON A.member_seqno = E.member_seqno ";
        $query .= " INNER JOIN order_file AS F ON A.order_common_seqno = F.order_common_seqno ";
        $query .= " LEFT JOIN order_dlvr AS G ON A.order_common_seqno = G.order_common_seqno AND G.tsrs_dvs = '수신' ";
        $query .= " LEFT JOIN empl AS H ON B.receipt_mng = H.empl_id ";
        $query .= " WHERE A.order_num = %s ";

        CLS_File::FileWrite(LOGPATH, "query : " . $query ,"a+");
        $query  = sprintf($query, $param['order_num']);


        return $conn->Execute($query);
    }

    public function selectWorkDetailByCount($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $masks = array();
        if(array_key_exists('masks',$param))
            $masks = explode('|',$param['masks']);

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "SELECT /* 수동접수 - 상세정보 얻기 (건별) */ ";
        $query .= " A.order_detail_file_num, C.title, C.cate_sortcode, F.cate_name, D.member_id,  ";
        $query .= " D.member_name, G.name as paper_name, G.color, G.basisweight, B.stan_name, B.order_detail, ";
        $query .= " B.print_tmpt_name, C.amt, E.empl_id, E.name as empl_name, B.state, H.name as receive_name, H.cell_num, H.addr, H.addr_detail, ";
        $query .= " A.file_path, A.save_file_name, B.side_dvs, C.receipt_finish_date, H.dlvr_way, H.dlvr_sum_way, H.invo_cpn, H.invo_num ";
        $query .= " FROM order_detail_count_file AS A ";
        $query .= " INNER JOIN order_detail AS B ON A.order_detail_seqno = B.order_detail_seqno ";
        $query .= " INNER JOIN order_common AS C ON B.order_common_seqno = C.order_common_seqno ";
        $query .= " INNER JOIN member AS D ON C.member_seqno = D.member_seqno ";
        $query .= " LEFT JOIN empl AS E ON E.empl_id = C.receipt_mng ";
        $query .= " INNER JOIN cate AS F ON F.sortcode = C.cate_sortcode ";
        $query .= " INNER JOIN cate_paper AS G ON G.mpcode = B.cate_paper_mpcode ";
        $query .= " INNER JOIN order_dlvr AS H ON H.order_common_seqno = C.order_common_seqno AND H.tsrs_dvs = '수신' ";
        $query .= " WHERE A.order_detail_file_num = %s ";

        CLS_File::FileWrite(LOGPATH, "query : " . $query ,"a+");
        $query  = sprintf($query, $param['item_id']);


        return $conn->Execute($query);
    }

    public function selectTypsetNumFromOrderNum($conn, $param) {
        $query = "\n    SELECT typset_num ";
        $query .= "\n   FROM order_detail_count_file AS A";
        $query .= "\n    INNER JOIN amt_order_detail_sheet AS B ON A.order_detail_count_file_seqno = B.order_detail_count_file_seqno ";
        $query .= "\n   INNER JOIN sheet_typset AS C ON B.sheet_typset_seqno = C.sheet_typset_seqno ";
        $query .= "\n   where A.order_detail_file_num like '%". $param["order_num"] ."%'";

        CLS_File::FileWrite(LOGPATH, "query : " . $query ,"a+");
        $rs = $conn->Execute($query);

        $result['id'] = array();
        while ($rs && !$rs->EOF) {
            array_push($result['id'], $rs->fields['typset_num']);
            $rs->MoveNext();
        }

        return $result['id'];
    }

    public function selectAutoSet($conn, $param) {
        $query = "\n    SELECT * /* 자동접수 - 설정정보 */ ";
        $query .= "\n   FROM auto_set LIMIT 1";

        return $conn->Execute($query);
    }

    public function getPrintfilepath($conn, $order_num) {
        $query = "\n    SELECT B.typset_way, B.outsource_etprs_cate, A.count, C.order_detail_dvs_num ";
        $query .= "\n   FROM order_common AS A ";
        $query .= "\n   INNER JOIN cate AS B ON A.cate_sortcode = B.sortcode ";
        $query .= "\n   INNER JOIN order_detail AS C ON A.order_common_seqno = C.order_common_seqno ";
        $query .= "\n   WHERE order_num = %s ";

        $query = sprintf($query, $order_num);

        $rs = $conn->Execute($query);

        $path_result = array();
        $print_file_path = "";
        if($rs != null) {
            $typset_way = $rs->fields['typset_way'];
            $outsource_etprs_cate = $rs->fields['outsource_etprs_cate'];

            if($typset_way == "CYPRESS") {
                $print_file_path = "/CYPRESS/input/";
            } else if($typset_way == "OUTSOURCE") {
                $print_file_path = "/OUTSOURCE/" . $outsource_etprs_cate . "/"
                    . date("Y/m/d", mktime(0,0,0,date("m")  , date("d"), date("Y"))) . "/";
            } else if($typset_way == "AGFA") {
                $print_file_path = "/AGFA/" . date("Y/m/d", mktime(0,0,0,date("m")  , date("d"), date("Y")));
            }
        }
        $path_result['print_file_path'] = $print_file_path;
        $path_result['count'] = $rs->fields['count'];
        $path_result['order_detail_dvs_num'] = $rs->fields['order_detail_dvs_num'];
        return $path_result;
    }

    public function updateTypsetCompleteByCount($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $order_detail_file_nums = $this->arr2paramStr($conn, $param['order_detail_file_nums']);
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n   UPDATE order_detail_count_file AS A  ";
        $query .= "\n  INNER JOIN amt_order_detail_sheet AS B ON A.order_detail_count_file_seqno = B.order_detail_count_file_seqno ";
        $query .= "\n  INNER JOIN sheet_typset AS C ON C.typset_num = %s ";
        $query .= "\n  SET B.sheet_typset_seqno = C.sheet_typset_seqno ";
        $query .= "\n  WHERE A.order_detail_file_num IN (%s) ";

        $query = sprintf($query, $param['typset_num'] ,$order_detail_file_nums);

        $conn->Execute($query);
    }

    public function  selectOrderDetailFileNums($conn, $param) {
        $query = "\n    SELECT * FROM order_detail_count_file ";
        $query .= "\n   where order_detail_file_num like '%" . $param['order_id'] . "%' ";

        return $conn->Execute($query);
    }

    public function selectOrderDetailFileNumState($conn, $param) {
        $query = "\n    SELECT * FROM order_detail_count_file ";
        $query .= "\n   where order_detail_file_num = '" . $param['order_detail_file_num'] . "' ";

        $rs = $conn->Execute($query);

        while($rs && !$rs->EOF) {
            return $rs->fields['state'];
        }
    }

    public function updateOrderDetailCountFileInfo($conn, $param) {
        $update_param = array();
        $update_param["table"] = "order_detail_count_file";
        $update_param["col"]["file_path"] = $param['file_path'];
        $update_param["col"]["print_file_path"] = $param['print_file_path'];
        $update_param["col"]["save_file_name"] = $param['save_file_name'];
        $update_param["col"]["print_file_name"] = $param['print_file_name'];
        $update_param["prk"] = "order_detail_file_num";
        $update_param["prkVal"] = $param['order_detail_file_num'];

        CLS_File::FileWrite(LOGPATH, "file_path : " . $param['file_path'] ,"a+");
        CLS_File::FileWrite(LOGPATH, "print_file_path : " . $param['print_file_path'] ,"a+");
        CLS_File::FileWrite(LOGPATH, "save_file_name : " . $param['save_file_name'] ,"a+");
        CLS_File::FileWrite(LOGPATH, "print_file_name : " . $param['print_file_name'] ,"a+");
        CLS_File::FileWrite(LOGPATH, "order_detail_file_num : " . $param['order_detail_file_num'] ,"a+");

        $this->updateData($conn, $update_param);
    }

    public function selectAworkForAuto($conn, $param) {
        $query = " SELECT /* 자동접수 - 주문상세정보 */ ";
        $query .= " A.order_num, A.title, A.order_common_seqno, ";
        $query .= " A.cate_sortcode, C.cate_name, ";
        $query .= " A.count, A.order_regi_date, ";
        $query .= " B.work_size_wid, B.work_size_vert, B.cut_size_wid, B.cut_size_vert, ";
        $query .= " B.stan_name, B.side_dvs, B.tot_tmpt, B.amt, A.cust_memo, D.file_path, D.save_file_name, B.receipt_mng ";
        $query .= " FROM order_common AS A ";
        $query .= " INNER JOIN order_detail AS B ON A.order_common_seqno = B.order_common_seqno ";
        $query .= " INNER JOIN cate AS C ON A.cate_sortcode = C.sortcode ";
        $query .= " INNER JOIN order_file AS D ON A.order_common_seqno = D.order_common_seqno ";
        $query .= " WHERE A.order_state = '1320' AND A.receipt_dvs = 'Auto' ";
        $query .= " ORDER BY A.order_regi_date  ";
        $query .= " LIMIT 1 ";

        CLS_File::FileWrite(LOGPATH, "query : " . $query ,"a+");
        return $conn->Execute($query);
    }

    public function updateToReceiptingState($conn, $seqno) {

    }

    public function selectOrderCommonState($conn, $order_num) {
        $query = "SELECT order_state";
        $query .= " FROM order_common";
        $query .= " WHERE order_num = '%s'";

        $query = sprintf($query, $order_num);

        $rs = $conn->Execute($query);
        return $rs->fields['order_state'];
    }

    public function isWorkPossessedCommercial($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT A.order_state, C.empl_id, C.name FROM order_common AS A ";
        $query .= "\n   INNER JOIN order_detail AS B ON A.order_common_seqno = B.order_common_seqno ";
        $query .= "\n   LEFT JOIN empl AS C ON B.receipt_mng = C.empl_id ";
        $query.= "\n    WHERE A.order_num = %s ";

        $query = sprintf($query, $param['order_num']);
        return $conn->Execute($query);
    }

    public function selectPaperList($conn) {
        $query = "\n    select distinct(concat(name, ' ' ,color, ' ', basisweight)) as paper from cate_paper ";
        return $conn->Execute($query);
    }

    public function selectCommercialPlateWatingList($conn, $param) {
        $query = " SELECT /* 상업인쇄 조판대기 리스트 */ ";
        $query .= " A.order_detail_file_num, C.title, C.order_common_seqno, F.sortcode, F.cate_name, E.member_id, E.member_name, B.side_dvs, B.tot_tmpt, B.amt, G.name, G.color, G.basisweight, C.order_regi_date ";
        $query .= " ,A.file_path, A.save_file_name, C.amt_unit_dvs, B.stan_name, B.receipt_finish_date, B.produce_memo,  B.print_tmpt_name ";
        $query .= " from order_detail_count_file AS A ";
        $query .= " INNER JOIN order_detail AS B ON A.order_detail_seqno = B.order_detail_seqno ";
        $query .= " INNER JOIN order_common AS C ON B.order_common_seqno = C.order_common_seqno ";
        $query .= " INNER JOIN amt_order_detail_sheet AS D ON D.order_detail_count_file_seqno = A.order_detail_count_file_seqno ";
        $query .= " INNER JOIN member AS E ON E.member_seqno = C.member_seqno ";
        $query .= " INNER JOIN cate AS F ON C.cate_sortcode = F.sortcode ";
        $query .= " INNER JOIN cate_paper AS G ON G.mpcode = B.cate_paper_mpcode ";
        $query .= " WHERE D.sheet_typset_seqno IS NULL and C.commerce_dvs = '상업' and C.order_state = '2120' ";

        CLS_File::FileWrite(LOGPATH, "query : " . $query ,"a+");
        return $conn->Execute($query);
    }

    public function selectWorkListForQC($conn, $param) {
        $query = " SELECT /* 자동접수 - 아포지 완료목록 */ ";
        $query .= " A.order_num, A.title, A.order_common_seqno, ";
        $query .= " A.cate_sortcode, C.cate_name, ";
        $query .= " A.count, A.order_regi_date, A.order_state, ";
        $query .= " B.work_size_wid, B.work_size_vert, B.cut_size_wid, B.cut_size_vert, E.order_detail_file_num, ";
        $query .= " B.stan_name, B.side_dvs, B.tot_tmpt, B.amt, A.cust_memo, D.file_path, D.save_file_name, B.receipt_mng, E.seq, ";
        $query .= " F.member_id, F.member_name ";
        $query .= " FROM order_common AS A ";
        $query .= " INNER JOIN order_detail AS B ON A.order_common_seqno = B.order_common_seqno ";
        $query .= " INNER JOIN cate AS C ON A.cate_sortcode = C.sortcode ";
        $query .= " INNER JOIN order_file AS D ON A.order_common_seqno = D.order_common_seqno ";
        $query .= " INNER JOIN order_detail_count_file AS E ON E.order_detail_seqno = B.order_detail_seqno ";
        $query .= " INNER JOIN member AS F ON F.member_seqno = A.member_seqno ";

        if($param['filters'] == 'ready') {
            $query .= " WHERE E.state = '1340' ";
            //$query .= " WHERE E.state = '1350' AND A.qc_check_pc = '" . $param['qc_check_pc'] . "' ";
        } else {
            $query .= " WHERE E.state = '1350' AND A.qc_check_pc = '" . $param['qc_check_pc'] . "' ";
        }

        if(array_key_exists('categories',$param))
        {
            if(strpos($param['categories'],"others") !== false) {
                $query .= " AND (A.order_num NOT LIKE '%NC%' AND  A.order_num NOT LIKE '%BL%' AND A.order_num NOT LIKE '%ST%' AND A.order_num NOT LIKE '%EV%') ";
            } else {
                $query .= " AND ( 1 != 1 ";
                if(strpos($param['categories'], 'N') !== false) {
                    $query .= " OR A.order_num LIKE '%NC%' ";
                }
                if(strpos($param['categories'], 'B') !== false) {
                    $query .= " OR A.order_num LIKE '%BL%' ";
                }
                if(strpos($param['categories'], 'S') !== false) {
                    $query .= " OR A.order_num LIKE '%ST%' ";
                }
                if(strpos($param['categories'], 'E') !== false) {
                    $query .= " OR A.order_num LIKE '%EV%' ";
                }
                $query .= " ) ";
            }
        }

        $query .= " ORDER BY A.order_state desc, order_regi_date ";

        if($param['filters'] == 'ready') {
            $query .= "\n   LIMIT 10 ";
        }
        CLS_File::FileWrite(LOGPATH, "query : " . $query ,"a+");
        return $conn->Execute($query);
    }

    public function selectWorkListForManualCommercial($conn, $param) {
        $id = $param['empl_id'];
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $os_type = $param['os_type'];
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n SELECT /* 상업인쇄 수동접수 - 작업목록 얻기 */ ";
        $query .= "\n A.order_num, A.title, A.cate_sortcode, A.order_detail, A.order_common_seqno, A.order_state ";
        $query .= "\n  ,B.member_id, B.member_name, G.cate_name, B.onefile_etprs_yn ";
        $query .= "\n  ,C.side_dvs, A.count ,A.order_regi_date, C.receipt_mng, A.pay_price, D.save_file_name, D.file_path ";
        $query .= "\n  FROM order_common AS A ";
        $query .= "\n  INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= "\n  INNER JOIN order_detail AS C ON A.order_common_seqno = C.order_common_seqno ";
        $query .= "\n  INNER JOIN order_file AS D ON A.order_common_seqno = D.order_common_seqno ";
        $query .= "\n  INNER JOIN member_mng AS E ON A.member_seqno = E.member_seqno ";

        if($os_type == "MAC") {
            $query .= "\n INNER JOIN empl AS F ON F.empl_seqno = E.mac ";
            $query .= "\n INNER JOIN cate AS G ON A.cate_sortcode = G.sortcode ";
            $query .= "\n WHERE D.origin_file_name LIKE '%.sit' ";
        } else {
            $query .= "\n INNER JOIN empl AS F ON F.empl_seqno = E.ibm ";
            $query .= "\n INNER JOIN cate AS G ON A.cate_sortcode = G.sortcode ";
            $query .= "\n WHERE D.origin_file_name NOT LIKE '%.sit' ";
        }

        $query .= "\n AND A.commerce_dvs = '상업' ";

        // 임시코드
        $query .= "\n AND A.order_regi_date >= '2017-08-20' ";

        $query .= "\n AND (A.order_state = '1320' OR A.order_state = '1330') ";
        $query .= "\n ORDER BY order_regi_date ";


        CLS_File::FileWrite(LOGPATH, "query : " . $query ,"a+");
        return $conn->Execute($query);
    }

    public function ReceiptDvsOptionCheck($conn, $param) {
        $query = "\n   SELECT opt_name ";
        $query .= "\n  FROM order_opt_history ";
        $query .= "\n  WHERE order_common_seqno = " . $param['order_common_seqno'];

        $isAuto = true;

        $rs = $conn->Execute($query);
        $i = 0;
        while($rs && !$rs->EOF) {
            $i++;
            if($rs->fields['opt_name'] == "당일판") {
                $isAuto = true;
            } else {
                $isAuto = false;
            }
            $rs->MoveNext();
        }

        if($i > 1)
            $isAuto = false;

        return $isAuto;
    }

    public function updateReceiptDvs($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    UPDATE order_common  ";
        $query .= "\n   SET receipt_dvs = %s ";
        $query .= "\n   WHERE order_common_seqno = %s ";

        $query = sprintf($query, $param['receipt_dvs'], $param['order_common_seqno']);

        $conn->Execute($query);
    }

    public function selectNotDecidedReceiptDvsOrders($conn) {
        $query = "\n    SELECT A.order_common_seqno, A.member_seqno, flattyp_yn,  ";
        $query .= "\n   F.opt_name, opt_use_yn, after_use_yn, stan_name, file_upload_dvs, E.save_file_name,";
        $query .= "\n   A.cate_sortcode, onefile_etprs_yn ";
        $query .= "\n   from order_common AS A ";
        $query .= "\n   INNER JOIN order_detail AS B ON A.order_common_seqno = B.order_common_seqno ";
        $query .= "\n   INNER JOIN cate AS C ON A.cate_sortcode = C.sortcode ";
        $query .= "\n   INNER JOIN member D ON A.member_seqno = D.member_seqno ";
        $query .= "\n   INNER JOIN order_file E ON E.order_common_seqno = A.order_common_seqno ";
        $query .= "\n   LEFT JOIN order_opt_history AS F ON A.order_common_seqno = F.order_common_seqno AND F.opt_name = '당일판' ";
        $query .= "\n   WHERE A.order_state = '1320' AND (A.receipt_dvs IS NULL OR A.receipt_dvs = '') ";

        return $conn->Execute($query);
    }

    public function selectWorkListForAll($conn, $param) {
        $count = $param['count'];
        $id = $param['empl_id'];
        $acceptors = $param['acceptors'];
        $accept_ready_date_range = explode("_", $param['accept_ready_date_range']);

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $os_type = $param['os_type'];
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "SELECT /* 수동접수 - 작업목록 얻기 */ ";
        $query .= "A.order_num, A.title, A.cate_sortcode, A.order_detail, A.order_common_seqno, A.order_state ";
        $query .= " ,B.member_id, B.member_name, G.cate_name, B.onefile_etprs_yn, A.owncompany_img_num, A.file_upload_dvs ";
        $query .= " ,C.side_dvs, A.count ,A.order_regi_date, C.receipt_mng, A.pay_price, D.save_file_name, D.file_path, F.empl_id, F.name as empl_name ";
        $query .= " FROM order_common AS A ";
        $query .= " INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= " INNER JOIN order_detail AS C ON A.order_common_seqno = C.order_common_seqno ";
        $query .= " INNER JOIN order_file AS D ON A.order_common_seqno = D.order_common_seqno ";
        $query .= " INNER JOIN member_mng AS E ON A.member_seqno = E.member_seqno ";

        if($os_type == "MAC") {
            $query .= " INNER JOIN empl AS F ON F.empl_id = C.receipt_mng ";
            $query .= " INNER JOIN cate AS G ON A.cate_sortcode = G.sortcode ";
            $query .= " WHERE D.origin_file_name LIKE '%.sit' ";
        } else {
            $query .= " INNER JOIN empl AS F ON F.empl_id = C.receipt_mng ";
            $query .= " INNER JOIN cate AS G ON A.cate_sortcode = G.sortcode ";
            $query .= " WHERE D.origin_file_name NOT LIKE '%.sit' ";
        }

        if(array_key_exists('categories',$param))
        {
            if(strpos($param['categories'],"others")) {
                $query .= " AND (A.order_num NOT LIKE '%NC%' AND  A.order_num NOT LIKE '%BL%' AND A.order_num NOT LIKE '%ST%' AND A.order_num NOT LIKE '%EV%') ";
            } else {
                $query .= " AND ( 1 != 1 ";
                if(strpos($param['categories'], 'N') !== false) {
                    $query .= " OR A.order_num LIKE '%NC%' ";
                }
                if(strpos($param['categories'], 'B') !== false) {
                    $query .= " OR A.order_num LIKE '%BL%' ";
                }
                if(strpos($param['categories'], 'S') !== false) {
                    $query .= " OR A.order_num LIKE '%ST%' ";
                }
                if(strpos($param['categories'], 'E') !== false) {
                    $query .= " OR A.order_num LIKE '%EV%' ";
                }
                $query .= " ) ";
            }
        }

        if(array_key_exists('customers',$param)) {
            if(strpos($param['customers'], 'designated')) {
                $query .= " AND A.order_mng = '" . $id . "' ";
            }
        }

        if(array_key_exists('accept_ready_date_range',$param)) {
            $query .= " AND C.receipt_start_date BETWEEN '" . $accept_ready_date_range[0]
                . "' AND '".$accept_ready_date_range[1]." 23:59:59' ";
        }

        $query .= " AND A.commerce_dvs = '일반' ";

        if($acceptors == 'me') {
            $query .= " AND C.receipt_mng = '" . $id . "'";
        }
        else if($acceptors == 'none') {
            $query .= " AND C.receipt_mng is null ";
        }
        else if($acceptors == 'auto') {
            $query .= " AND C.receipt_mng like 'auto%' ";
        }
        else if($acceptors == 'all_except_none') {
            $query .= " AND C.receipt_mng is not null ";
        }
        else if($acceptors == 'all_except_auto') {
            $query .= " AND C.receipt_mng not like 'auto%' ";
        }
        else if($acceptors == 'all_except_none_and_auto') {
            $query .= " AND C.receipt_mng not like 'auto%' AND C.receipt_mng is not null ";
        }

        $query .= " ORDER BY ";


        $query .= " CASE ";
        $query .= " WHEN C.receipt_mng = '" . $id . "' then 1 ";
        //$query .= " ELSE 2 END , A.order_regi_date desc ";
        $query .= " ELSE 2 END , A.order_regi_date ";

        if(!array_key_exists('customers',$param) && $count != -1) {
            $query .= " LIMIT " . $count . " ";
        } else {
            if(!strpos($param['customers'], 'designated') && $count != -1)
                $query .= " LIMIT " . $count . " ";
        }

        CLS_File::FileWrite(LOGPATH, "query : " . $query ,"a+");
        return $conn->Execute($query);
    }

    public function selectWorkListForManual($conn, $param) {
        $count = $param['count'];
        $id = $param['empl_id'];

        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $os_type = $param['os_type'];
        $param = $this->parameterArrayEscape($conn, $param);

        $query = "SELECT /* 수동접수 - 작업목록 얻기 */ ";
        $query .= "A.order_num, A.title, A.cate_sortcode, A.order_detail, A.order_common_seqno, A.order_state ";
        $query .= " ,B.member_id, B.member_name, G.cate_name, B.onefile_etprs_yn, A.owncompany_img_num, A.file_upload_dvs ";
        $query .= " ,C.side_dvs, A.count ,A.order_regi_date, C.receipt_mng, A.pay_price, D.save_file_name, D.file_path ";
        $query .= " FROM order_common AS A ";
        $query .= " INNER JOIN member AS B ON A.member_seqno = B.member_seqno ";
        $query .= " INNER JOIN order_detail AS C ON A.order_common_seqno = C.order_common_seqno ";
        $query .= " INNER JOIN order_file AS D ON A.order_common_seqno = D.order_common_seqno ";
        $query .= " INNER JOIN member_mng AS E ON A.member_seqno = E.member_seqno ";

        if($os_type == "MAC") {
            $query .= " INNER JOIN empl AS F ON F.empl_seqno = E.mac ";
            $query .= " INNER JOIN cate AS G ON A.cate_sortcode = G.sortcode ";
            $query .= " WHERE D.origin_file_name LIKE '%.sit' ";
        } else {
            $query .= " INNER JOIN empl AS F ON F.empl_seqno = E.ibm ";
            $query .= " INNER JOIN cate AS G ON A.cate_sortcode = G.sortcode ";
            $query .= " WHERE D.origin_file_name NOT LIKE '%.sit' ";
        }

        if(array_key_exists('categories',$param))
        {
            if(strpos($param['categories'],"others")) {
                $query .= " AND (A.order_num NOT LIKE '%NC%' AND  A.order_num NOT LIKE '%BL%' AND A.order_num NOT LIKE '%ST%' AND A.order_num NOT LIKE '%EV%') ";
            } else {
                $query .= " AND ( 1 != 1 ";
                if(strpos($param['categories'], 'N') !== false) {
                    $query .= " OR A.order_num LIKE '%NC%' ";
                }
                if(strpos($param['categories'], 'B') !== false) {
                    $query .= " OR A.order_num LIKE '%BL%' ";
                }
                if(strpos($param['categories'], 'S') !== false) {
                    $query .= " OR A.order_num LIKE '%ST%' ";
                }
                if(strpos($param['categories'], 'E') !== false) {
                    $query .= " OR A.order_num LIKE '%EV%' ";
                }
                $query .= " ) ";
            }
        }

        if(array_key_exists('customers',$param)) {
            if(strpos($param['customers'], 'designated')) {
                $query .= " AND A.order_mng = '" . $id . "' ";
            }
        }

        $query .= " AND A.commerce_dvs = '일반' ";

        $query .= " AND (A.order_state = '1320' OR ((A.order_state = '1330' OR A.order_state = '1370') AND C.receipt_mng = '" . $id . "'))";
        $query .= " AND A.receipt_dvs = 'Manual' ";

        $query .= " AND (C.receipt_mng = '" . $id . "' OR C.receipt_mng = '' OR C.receipt_mng IS NULL) ";
        $query .= " ORDER BY ";


        $query .= " CASE ";
        $query .= " WHEN F.empl_id = '" . $id . "' then 1 ";
        //$query .= " ELSE 2 END , A.order_regi_date desc ";
        $query .= " ELSE 2 END , A.order_regi_date ";

        if(!array_key_exists('customers',$param) && $count != -1) {
            $query .= " LIMIT " . $count . " ";
        } else {
            if(!strpos($param['customers'], 'designated') && $count != -1)
                $query .= " LIMIT " . $count . " ";
        }

        CLS_File::FileWrite(LOGPATH, "query : " . $query ,"a+");
        return $conn->Execute($query);
    }
}