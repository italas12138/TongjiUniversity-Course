package com.music.service;

import java.sql.SQLException;

import com.music.dao.AddCollectionDao;

public class AddCollectionService {

	public String addCollection(String uid, String sid,String method) throws NumberFormatException, SQLException {
		AddCollectionDao dao = new AddCollectionDao();
		String flag = dao.addCollection(uid,sid,method);
		return flag;
	}

}
