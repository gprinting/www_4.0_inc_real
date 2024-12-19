<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-08-15
 * Time: 오후 7:29
 */

include_once(INC_PATH . '/com/nexmotion/job/nimda/common/NimdaCommonDAO.inc');


class PlateFileDAO extends NimdaCommonDAO
{
    function __construct() {
    }

    function insertSheetTypsetFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    INSERT INTO sheet_typset_file ";
        $query .= "\n   (file_path, save_file_name, origin_file_name, sheet_typset_seqno) ";
        $query .= "\n   values ";
        $query .= "\n   (%s, %s, %s, %s) ";

        $query = sprintf($query,
            $param["file_path"],
            $param["save_file_name"],
            $param["origin_file_name"],
            $param["sheet_typset_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    function insertPreviewFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    INSERT INTO order_detail_count_preview_file ";
        $query .= "\n   (preview_file_path, preview_file_name, seq, order_detail_count_file_seqno) ";
        $query .= "\n   values ";
        $query .= "\n   (%s, %s, %s, %s) ";

        $query = sprintf($query,
            $param["preview_file_path"],
            $param["preview_file_name"],
            $param["seq"],
            $param["order_detail_count_file_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }


    function insertSheetTypsetLabelFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    INSERT INTO sheet_typset_label_file ";
        $query .= "\n   (file_path, save_file_name, origin_file_name, sheet_typset_seqno) ";
        $query .= "\n   values ";
        $query .= "\n   (%s, %s, %s, %s) ";

        $query = sprintf($query,
            $param["file_path"],
            $param["save_file_name"],
            $param["origin_file_name"],
            $param["sheet_typset_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    function insertSheetTypsetPreviewFile($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    INSERT INTO sheet_typset_preview_file ";
        $query .= "\n   (file_path, save_file_name, origin_file_name, sheet_typset_seqno) ";
        $query .= "\n   values ";
        $query .= "\n   (%s, %s, %s, %s) ";

        $query = sprintf($query,
            $param["file_path"],
            $param["save_file_name"],
            $param["origin_file_name"],
            $param["sheet_typset_seqno"]);

        $rs = $conn->Execute($query);

        return $rs;
    }

    function getNewReceipts($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    SELECT A.file_path, A.save_file_name, A.order_detail_file_num, A.order_detail_count_file_seqno ";
        $query .= "\n   FROM order_detail_count_file AS A ";
        $query .= "\n   LEFT JOIN order_detail_count_preview_file AS B ON A.order_detail_count_file_seqno = B.order_detail_count_file_seqno";
        $query .= "\n   WHERE A.print_file_path is not null and B.order_detail_count_file_seqno is null ";
        //$query .= "\n   WHERE A.print_file_path is not null and A.order_detail_count_file_seqno = 6776 ";

        $rs = $conn->Execute($query);

        $newly_made_files = array();
        $i = 0;
        while ($rs && !$rs->EOF) {
            $newly_made_files[$i]['file_path'] = $rs->fields['file_path'];
            $newly_made_files[$i]['file_name'] = $rs->fields['save_file_name'];
            $newly_made_files[$i]['order_detail_file_num'] = $rs->fields['order_detail_file_num'];
            $newly_made_files[$i]['order_detail_count_file_seqno'] = $rs->fields['order_detail_count_file_seqno'];
            $i++;
            $rs->MoveNext();
        }

        return $newly_made_files;
    }

    function getNewPlates($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $query = "\n    SELECT /* PlateFile - 새로만들어진 판정보 가져오기 */ ";
        $query .= "\n   sheet_typset_seqno, typset_num, paper_name, paper_dvs, paper_color, paper_basisweight, ";
        $query .= "\n   print_amt, print_amt_unit, dlvrboard, print_title, save_path, specialty_items ";
        $query .= "\n   FROM sheet_typset ";
        $query .= "\n   WHERE save_yn = 'N' ORDER BY sheet_typset_seqno ASC";

        $rs = $conn->Execute($query);

        $i = 0;
        $newly_made_files = array();
        while ($rs && !$rs->EOF) {
            $newly_made_files[$i]['sheet_typset_seqno'] = $rs->fields['sheet_typset_seqno'];
            $newly_made_files[$i]['typset_num'] = $rs->fields['typset_num'];
            $newly_made_files[$i]['paper_name'] = $rs->fields['paper_name'];
            $newly_made_files[$i]['paper_dvs'] = $rs->fields['paper_dvs'];
            $newly_made_files[$i]['paper_color'] = $rs->fields['paper_color'];
            $newly_made_files[$i]['paper_basisweight'] = $rs->fields['paper_basisweight'];
            $newly_made_files[$i]['print_amt'] = $rs->fields['print_amt'];
            $newly_made_files[$i]['print_amt_unit'] = $rs->fields['print_amt_unit'];
            $newly_made_files[$i]['dlvrboard'] = $rs->fields['dlvrboard'];
            $newly_made_files[$i]['print_title'] = $rs->fields['print_title'];
            $newly_made_files[$i]['save_path'] = $rs->fields['save_path'];
            $newly_made_files[$i]['specialty_items'] = $rs->fields['specialty_items'];

            $i++;
            $rs->MoveNext();
        }

        return $newly_made_files;
    }

    function getTypsetFormatInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT typset_format_seqno, format_name, subpaper, honggak_yn, paper, wid_size, vert_size ";
        $query .= "\n   FROM typset_format ";
        $query .= "\n   WHERE preset_name = %s ";

        $query = sprintf($query, $param["print_title"]);

        $rs = $conn->Execute($query);
        $typset_info = array();
        if ($rs && !$rs->EOF) {
            $typset_info["typset_format_seqno"] = $rs->fields["typset_format_seqno"];
            $typset_info["format_name"] = $rs->fields["format_name"];
            $typset_info["subpaper"] = $rs->fields["subpaper"];
            $typset_info["honggak_yn"] = $rs->fields["honggak_yn"];
            $typset_info["paper"] = $rs->fields["paper"];
            $typset_info["size"] = $rs->fields["wid_size"] . "*" . $rs->fields["vert_size"];
        }

        return $typset_info;
    }

    function updateComplete($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    UPDATE sheet_typset";
        $query .= "\n   SET save_yn = 'Y' ";
        $query .= "\n   WHERE  sheet_typset_seqno = %s ";

        $query = sprintf($query, $param["sheet_typset_seqno"]);
        return $conn->Execute($query);
    }

    function insertProduceOrd($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n        INSERT INTO produce_ord ";
        $query .= "\n       (dvs, ord_dvs, typset_num, paper, size, print_tmpt, amt, amt_unit, specialty_items) ";
        $query .= "\n       VALUES ";
        $query .= "\n       (%s, %s, %s, %s, %s, %s, %s, %s, %s) ";
        $query .= "\n       ON DUPLICATE KEY UPDATE ";
        $query .= "\n        dvs = %s, ";
        $query .= "\n        ord_dvs = %s, ";
        $query .= "\n        typset_num = %s, ";
        $query .= "\n        paper = %s, ";
        $query .= "\n        size = %s, ";
        $query .= "\n        print_tmpt = %s, ";
        $query .= "\n        amt = %s, ";
        $query .= "\n        amt_unit = %s, ";
        $query .= "\n        specialty_items = %s ";

        $query = sprintf($query,
            $param["dvs"],$param["ord_dvs"],$param["typset_num"],$param["paper"],$param["size"],$param["print_tmpt"],$param["amt"],$param["amt_unit"],$param["specialty_items"],
            $param["dvs"],$param["ord_dvs"],$param["typset_num"],$param["paper"],$param["size"],$param["print_tmpt"],$param["amt"],$param["amt_unit"],$param["specialty_items"]);

        return $conn->Execute($query);

    }
}