import { makeStyles } from '@material-ui/core/styles';

export default makeStyles((theme) => ({
    root: {
      width: '100%',
      '& > * + *': {
        marginTop: theme.spacing(2),
      },
    },
    filter_button: {
      borderRadius: 30,
      margin: '20px 0',
      display: 'flex',
      flexDirection: 'row',
      justifyContent: 'space-between',
      alignItems: 'center',
      padding: '10px 50px',
    },
  }));
  