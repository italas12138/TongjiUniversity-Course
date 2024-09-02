package com.music.service;

import java.sql.SQLException;

import com.music.dao.UpdateUserinfoDao;

public class UpdateUserinfoService {

	public String updateUserinfo(String uid, String attr, String newValue) throws NumberFormatException, SQLException {
		UpdateUserinfoDao dao = new UpdateUserinfoDao();
		String flag = dao.updateUserinfo(uid,attr,newValue);
		return flag;
	}

}
