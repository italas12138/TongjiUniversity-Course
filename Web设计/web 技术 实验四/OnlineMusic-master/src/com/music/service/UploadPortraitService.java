package com.music.service;

import java.sql.SQLException;

import com.music.dao.UploadPortraitDao;

public class UploadPortraitService {

	public String upload(String portraitUrl,String userName) throws SQLException {
		UploadPortraitDao dao = new UploadPortraitDao();
		String flag = dao.upload(portraitUrl,userName);
		return flag;
	}

}
