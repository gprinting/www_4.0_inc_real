<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-12-04
 * Time: 오후 6:47
 */

include_once(INC_PATH . '/com/nexmotion/job/nimda/common/NimdaCommonDAO.inc');
include_once(INC_PATH . "/common_lib/cypress_file.inc");

$curDirectory = dirname(__FILE__);
define("LOGPATH", $curDirectory . "/logs/" . date("Y_m_d"));

class LabelManagerDAO extends NimdaCommonDAO {
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

    public function selectAWork($conn) {
        $query = " SELECT ";
        $query .= "\n C.order_common_seqno ,A.order_detail_file_num, C.title, ";
        $query .= "\n C.order_detail, D.mail ,D.member_name, E.sortcode, E.cate_name, ";
        $query .= "\n F.addr, F.addr_detail, B.cut_size_wid, B.cut_size_vert, C.cust_memo, C.amt, C.amt_unit_dvs, C.count, ";
        $query .= "\n C.expec_weight, F.dlvr_way, F.dlvr_sum_way, J.typset_num, J.print_title, G.empl_id, ";
        $query .= "\n H.preview_file_path, H.preview_file_name ";
        $query .= "\n FROM order_detail_count_file AS A ";
        $query .= "\n INNER JOIN order_detail AS B ON A.order_detail_seqno = B.order_detail_seqno ";
        $query .= "\n INNER JOIN order_common AS C ON B.order_common_seqno = C.order_common_seqno ";
        $query .= "\n INNER JOIN member AS D ON C.member_seqno = D.member_seqno ";
        $query .= "\n INNER JOIN cate AS E ON C.cate_sortcode = E.sortcode ";
        $query .= "\n INNER JOIN order_dlvr AS F ON C.order_common_seqno = F.order_common_seqno AND F.tsrs_dvs = '수신' ";
        $query .= "\n LEFT JOIN empl AS G ON C.receipt_mng = G.empl_id ";
        $query .= "\n LEFT JOIN order_detail_count_preview_file AS H ON A.order_detail_count_file_seqno = H.order_detail_count_file_seqno";
        $query .= "\n LEFT JOIN amt_order_detail_sheet AS I ON I.order_num = C.order_num";
        $query .= "\n LEFT JOIN sheet_typset AS J ON J.sheet_typset_seqno = I.sheet_typset_seqno";
        $query .= "\n WHERE A.label_file_path IS NULL AND A.state >= '2210' ";
        $query .= "\n LIMIT 1 ";

        return $conn->Execute($query);
    }

    public function selectAPInfo($conn, $order_common_seqno) {
        $query = " SELECT * ";
        $query .= " FROM order_after_history ";
        $query .= " WHERE order_common_seqno = '%s' AND basic_yn = 'N' ";

        $query = sprintf($query, $order_common_seqno);

        $rs = $conn->Execute($query);

        $ap_str = "";
        while($rs && !$rs->EOF) {
            if($ap_str == "") {
                $ap_str = $rs->fields['after_name'];
            } else {
                $ap_str .= " / " . $rs->fields['after_name'];
            }
            $rs->MoveNext();
        }

        return $ap_str;
    }

    public function update($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = " UPDATE order_detail_count_file ";
        $query .= "\n SET label_file_path = %s ";
        $query .= "\n ,label_file_name = %s ";
        $query .= "\n  WHERE order_detail_file_num = %s";

        $query = sprintf($query,
            $param['order_detail_file_num'],
            $param['order_detail_file_num'],
            $param['order_detail_file_num']);

        $conn->Execute($query);
    }
}