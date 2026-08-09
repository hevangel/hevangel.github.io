
					for (i=0; i<tempclass->studentno; i++)
						if (strcpy(tempclass->studentdata[i].stcode, curstudent->data.stcode) == 0)
						{
							for (j=i;j<tempclass->studentno-1;j++)
								tempclass->studentdata[j] = tempclass->studentdata[j+1];
						}
					tempclass->studentno--;

					fp = fopen(szBuffer, "wb");
					fwrite(tempclass, sizeof(classrec), 1, fp);
					fclose(fp);
					delete tempclass;
				}

				delete curstudent;

//Set total class no box.
				wsprintf(szBuffer,"Total students : %d", studentno);
				SetWindowText(hControl[1], szBuffer);
				ListBox_DeleteString(hControl[0],indexpos);
				ListBox_SetCurSel(hControl[0],indexpos);
				SetFocus(hControl[0]);


	wsprintf(szBuffer, "subcode : %s\n"
				"section : %d\n"
				"teacher : %s\n"
				"description : %s\n"
				"%s, %s, %d\n"
				"%s, %d, %d, %s\n",
				tempclass->subcode, tempclass->section,
				tempclass->description,
				tempclass->teachcode,
				tempclass->studentdata[30].lastname,
				tempclass->studentdata[30].firstname,
				tempclass->studentdata[30].marks[56],
				tempclass->fielddata[70].description,
				tempclass->fielddata[70].weight,
				tempclass->fielddata[70].type,
				tempclass->fielddata[70].range);
	MessageBox(NULL, szBuffer, "Hello", MB_OK);

