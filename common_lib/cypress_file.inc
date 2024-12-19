<?
 /***********************************************************************************
 *** 프로 젝트 : CyPress 2
 *** 개발 영역 :  File Lib
 *** 개  발  자 : 김성진
 *** 개발 날짜 : 2016.06.15
 ***********************************************************************************/

 class CLS_File {

	static function FileRead($fileName) {
		if (!file_exists($fileName)) {
			echo "파일이 존재하지 않습니다.";
		} else {
		  $fp = fopen($fileName, "r");
	      if(!$fp) echo "파일을 여는데 실패했습니다. 다시 확인하시길 바랍니다.";
		  $content = fread($fp, filesize($fileName));
		  fclose($fp);

		  return $content;
		}
	}

	 static function FileList($path) {
		 $fileName = array();
		 if ($handle = opendir($path."/")) {
			 $nCount = 0;
			 while ($file = readdir($handle)) {
				 if ($file != "." && $file != "..") {
					 array_push($fileName, $file);
				 }
			 }
		 }
		 closedir($handle);

		 return $fileName;
	 }

	static function FileWrite($fileName, $content, $mode) {
		if (!$fileName || !$content) {
			echo "파일명 또는 내용을 입력하세요!!";
		} else {
			$fp = fopen($fileName, $mode);
			if(!$fp) echo "파일을 여는데 실패했습니다. 다시 확인하시길 바랍니다.";
			fwrite($fp, $content . '\n\n\n');
			fclose($fp);
		}
	}

 	static function FileDelete($fileName) {
		if (!file_exists($fileName)) {
			echo "파일이 존재하지 않습니다.";
		} else {
			@chmod($fileName, 0775);
			$handle = @unlink($fileName);
			return $handle;
		}
	}


 	static function MKdirCreate($dir, $chown) {
		 if(mkdir($dir, $chown)) {
			 if(is_dir($dir)) {
				 chmod($dir, $chown);
				 $nRes = "SUCCESS";
			 } else {
				 $nRes = "FAILED";
			 }
		 } else {
			 $nRes = "FAILED";
		 }

		 return $nRes;
	 }


	 static function FileCopy($oldfile, $newfile) {
		 if(file_exists($oldfile)) {
			 if(!copy($oldfile, $newfile)) {
				 $nRes = "FAILED";
			 } else if(file_exists($newfile)) {
				 $nRes = "SUCCESS";
			 }
		 } else {
			 $nRes = "FAILED";
		 }

		 return $nRes;
	 }

}
?>